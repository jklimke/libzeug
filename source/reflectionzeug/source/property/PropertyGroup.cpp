
#include <cassert>
#include <algorithm>

#include <reflectionzeug/property/PropertyGroup.h>

#include <reflectionzeug/property/AbstractValueProperty.h>
#include <reflectionzeug/property/AbstractVisitor.h>
#include <reflectionzeug/tools/SerializerJSON.h>

#include <stringzeug/manipulation.h>


namespace reflectionzeug
{


const char PropertyGroup::s_separator('.');


PropertyGroup::PropertyGroup()
: AbstractProperty("")
, m_ownsProperties(true)
{
}

PropertyGroup::PropertyGroup(const std::string & name)
: AbstractProperty(name)
, m_ownsProperties(true)
{
}

PropertyGroup::~PropertyGroup()
{
    // Delete owned properties
    if (m_ownsProperties)
    {
        for (AbstractProperty * property : m_properties)
        {
            delete property;
        }
    }
}

void PropertyGroup::clear()
{
    // Remove all properties
    auto it = m_properties.begin();
    while (it != m_properties.end())
    {
        // Get property index
        size_t index = std::distance(m_properties.begin(), it);

        // Invoke callback
        beforeRemove(index);

        // Delete property
        if (m_ownsProperties)
        {
            AbstractProperty * property = *it;
            delete property;
        }

        // Remove property
        m_propertiesMap.erase((*it)->name());
        it = m_properties.erase(it);

        // Invoke callback
        afterRemove(index);
    }

    // Make sure that property list is empty
    assert(m_properties.empty());
    assert(m_propertiesMap.empty());
}

void PropertyGroup::setOwnsProperties(bool owns)
{
    m_ownsProperties = owns;
}

const std::unordered_map<std::string, AbstractProperty *> & PropertyGroup::properties() const
{
    return m_propertiesMap;
}

bool PropertyGroup::propertyExists(const std::string & name) const
{
    return m_propertiesMap.find(name) != m_propertiesMap.end();
}

AbstractProperty * PropertyGroup::property(const std::string & path)
{
    std::vector<std::string> splittedPath = stringzeug::split(path, s_separator);
    return const_cast<AbstractProperty *>(findProperty(splittedPath));
}

const AbstractProperty * PropertyGroup::property(const std::string & path) const
{
    std::vector<std::string> splittedPath = stringzeug::split(path, s_separator);
    return findProperty(splittedPath);
}

bool PropertyGroup::groupExists(const std::string & name) const
{
    return this->propertyExists(name) &&
           m_propertiesMap.at(name)->as<PropertyGroup>() != nullptr;
}

PropertyGroup * PropertyGroup::group(const std::string & path)
{
    // Get property by path
    AbstractProperty * property = this->property(path);
    if (!property) {
        return nullptr;
    }

    // Convert into group
    return property->as<PropertyGroup>();
}

const PropertyGroup * PropertyGroup::group(const std::string & path) const
{
    // Get property by path
    const AbstractProperty * property = this->property(path);
    if (!property) {
        return nullptr;
    }

    // Convert into group
    return property->as<PropertyGroup>();
}

AbstractProperty * PropertyGroup::addProperty(AbstractProperty * property)
{
    // Reject properties that have no name or whose name already exists
    if (!property || !property->hasName() || this->propertyExists(property->name()))
    {
        return nullptr;
    }

    // Invoke callback
    beforeAdd(count(), property);

    // Add property
    m_properties.push_back(property);
    m_propertiesMap.insert(std::make_pair(property->name(), property));

    // Invoke callback
    afterAdd(count(), property);

    // Return property
    return property;
}

PropertyGroup * PropertyGroup::addGroup(const std::string & name)
{
    // Create group
    PropertyGroup * group = new PropertyGroup(name);
    if (!this->addProperty(group))
    {
        // Abort, if group could not be added
        delete group;
        return nullptr;
    }

    return group;
}

PropertyGroup * PropertyGroup::ensureGroup(const std::string & path)
{
    std::vector<std::string> splittedPath = stringzeug::split(path, s_separator);
    return ensureGroup(splittedPath);
}

AbstractProperty * PropertyGroup::takeProperty(const std::string & name)
{
    // Check if property exists in this group
    if (!this->propertyExists(name))
    {
        return nullptr;
    }

    // Get property and property index
    AbstractProperty * property = m_propertiesMap.at(name);
    auto it = std::find(m_properties.begin(), m_properties.end(), property);

    assert(it != m_properties.end());

    size_t index = indexOf(*it);

    // Invoke callback
    beforeRemove(index);

    // Remove property from group
    m_properties.erase(it);
    m_propertiesMap.erase(name);

    // Invoke callback
    afterRemove(index);

    // Return property
    return property;
}

void PropertyGroup::forEachValue(const std::function<void(AbstractValueProperty &)> & callback)
{
    // Visit all values
    for (AbstractProperty * property : m_properties)
    {
        // Check if property is a value property
        AbstractValueProperty * value = dynamic_cast<AbstractValueProperty *>(property);
        if (value) {
            callback(*value);
        }
    }
}

void PropertyGroup::forEachValue(const std::function<void(const AbstractValueProperty &)> & callback) const
{
    // Visit all values
    for (const AbstractProperty * property : m_properties)
    {
        // Check if property is a value property
        const AbstractValueProperty * value = dynamic_cast<const AbstractValueProperty *>(property);
        if (value) {
            callback(*value);
        }
    }
}

void PropertyGroup::forEachCollection(const std::function<void(AbstractCollection &)> & callback)
{
    // Visit all collections
    for (AbstractProperty * property : m_properties)
    {
        // Check if property is a collection
        AbstractCollection * collection = dynamic_cast<AbstractCollection *>(property);
        if (collection) {
            callback(*collection);
        }
    }
}

void PropertyGroup::forEachCollection(const std::function<void(const AbstractCollection &)> & callback) const
{
    // Visit all collections
    for (const AbstractProperty * property : m_properties)
    {
        // Check if property is a collection
        const AbstractCollection * collection = dynamic_cast<const AbstractCollection *>(property);
        if (collection) {
            callback(*collection);
        }
    }
}

void PropertyGroup::forEachGroup(const std::function<void(PropertyGroup &)> & callback)
{
    // Visit all groups
    for (AbstractProperty * property : m_properties)
    {
        // Check if property is a group
        PropertyGroup * group = dynamic_cast<PropertyGroup *>(property);
        if (group) {
            callback(*group);
        }
    }
}

void PropertyGroup::forEachGroup(const std::function<void(const PropertyGroup &)> & callback) const
{
    // Visit all groups
    for (const AbstractProperty * property : m_properties)
    {
        // Check if property is a group
        const PropertyGroup * group = dynamic_cast<const PropertyGroup *>(property);
        if (group) {
            callback(*group);
        }
    }
}

const std::type_info & PropertyGroup::type() const
{
    return typeid(PropertyGroup);
}

Variant PropertyGroup::toVariant() const
{
    // Create variant map from all properties in the group
    Variant map = Variant::map();
    for (auto it : m_propertiesMap) {
        // Get name and property
        std::string        name = it.first;
        AbstractProperty * prop = it.second;

        // Add to variant map
        (*map.asMap())[name] = prop->toVariant();
    }

    // Return variant representation
    return map;
}

bool PropertyGroup::fromVariant(const Variant & value)
{
    // Check if variant is a map
    if (!value.isMap()) {
        return false;
    }

    // Get all values from variant map
    for (auto it : *value.asMap()) {
        // Get name and value
        std::string     name = it.first;
        const Variant & var  = it.second;

        // If this names an existing property, set its value
        AbstractProperty * prop = this->property(name);
        if (prop) {
            prop->fromVariant(var);
        }
    }

    // Done
    return true;
}

std::string PropertyGroup::toString() const
{
    // Convert group into JSON
    SerializerJSON json;
    return json.toString(this->toVariant());
}

bool PropertyGroup::fromString(const std::string & string)
{
    // Convert from JSON
    Variant values;
    SerializerJSON json;
    if (json.fromString(values, string)) {
        return fromVariant(values);
    }

    // Error
    return false;
}

void PropertyGroup::accept(AbstractVisitor * visitor)
{
    visitor->callVisitor<PropertyGroup>(this);
    visitor->callVisitor<AbstractCollection>(this);
}

bool PropertyGroup::isEmpty() const
{
    return m_properties.size() == 0;
}

size_t PropertyGroup::count() const
{
    return m_properties.size();
}

AbstractProperty * PropertyGroup::at(size_t index)
{
    // Check if index is valid
    if (index >= this->count())
    {
        return nullptr;
    }

    // Return property
    return m_properties[index];
}

const AbstractProperty * PropertyGroup::at(size_t index) const
{
    // Check if index is valid
    if (index >= this->count())
    {
        return nullptr;
    }

    // Return property
    return m_properties[index];
}

int PropertyGroup::indexOf(const AbstractProperty * property) const
{
    // Check input parameter
    assert(property);

    // Check if property exists in the group
    if (!this->propertyExists(property->name()))
    {
        return -1;
    }

    // Return index of property
    return (int)std::distance(m_properties.begin(), std::find(m_properties.begin(), m_properties.end(), property));
}

void PropertyGroup::forEach(const std::function<void(AbstractProperty &)> & callback)
{
    // Invoke callback for each property
    for (AbstractProperty * property : m_properties)
    {
        callback(*property);
    }
}

void PropertyGroup::forEach(const std::function<void(const AbstractProperty &)> & callback) const
{
    // Invoke callback for each property
    for (AbstractProperty * property : m_properties)
    {
        // Invoke callback
        callback(*property);
    }
}

const AbstractProperty * PropertyGroup::findProperty(const std::vector<std::string> & path) const
{
    // [TODO] Use iterative approach rather than recursion

    // Check if path is valid
    if (path.size() == 0) {
        return nullptr;
    }

    // Check if first element of the path exists in this group
    if (!propertyExists(path.front())) {
        return nullptr;
    }

    // Get the respective property
    AbstractProperty * property = m_propertiesMap.at(path.front());

    // If there are no more sub-paths, return the found property
    if (path.size() == 1) {
        return property;
    }

    // Otherwise, it is an element in the middle of the path, so ensure it is a group
    if (!property->isGroup()) {
        return nullptr;
    }

    // Call recursively on subgroup
    return property->asGroup()->findProperty({ path.begin() + 1, path.end() });
}

PropertyGroup * PropertyGroup::ensureGroup(const std::vector<std::string> & path)
{
    // [TODO] Use iterative approach rather than recursion

    // Check if path is valid
    if (path.size() == 0) {
        return nullptr;
    }

    // Check if group exists
    PropertyGroup * group = nullptr;
    if (propertyExists(path.front()))
    {
        // Get property
        AbstractProperty * property = m_propertiesMap.at(path.front());

        // Abort if this is not a group
        if (!property->isGroup()) {
            return nullptr;
        }

        // Get as group
        group = property->asGroup();
    }
    else
    {
        // Add new group
        group = addGroup(path.front());
    }

    // If there are no more sub-paths, return the group
    if (path.size() == 1) {
        return group;
    }

    // Otherwise, call recursively on subgroup
    return group->ensureGroup(std::vector<std::string>(path.begin() + 1, path.end()));
}


} // namespace reflectionzeug
