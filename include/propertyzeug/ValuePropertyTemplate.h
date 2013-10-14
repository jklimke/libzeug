
#pragma once

#include <memory>

#include <signalzeug/Signal.h>

#include <propertyzeug/propertyzeug.h>
#include <propertyzeug/ValueProperty.h>
#include <propertyzeug/StoredValue.h>
#include <propertyzeug/AccessorValue.h>

namespace zeug {
    
/** @brief
 * Part of the property hierarchy.
 * The Template Class for all properties that have a value.
 */

template <typename Type>
class ValuePropertyTemplate : public ValueProperty
{
public:
    ValuePropertyTemplate(const std::string & name, const Type & value);
    
    ValuePropertyTemplate(const std::string & name, 
                          const std::function<Type()> & getter,
                          const std::function<void(const Type &)> & setter);
    
    template <class Object>
    ValuePropertyTemplate(const std::string & name,
                          Object & object, const Type & (Object::*getter_pointer)() const,
                          void (Object::*setter_pointer)(const Type &));
    
    template <class Object>
    ValuePropertyTemplate(const std::string & name,
                          Object & object, Type (Object::*getter_pointer)() const,
                          void (Object::*setter_pointer)(const Type &));
    
    virtual ~ValuePropertyTemplate();

    virtual Type value() const;
    virtual void setValue(const Type & value);
    
    Signal<const Type &> valueChanged;
protected:
    std::unique_ptr<AbstractValue<Type>> m_value;
};

} // namespace zeug

#include "ValuePropertyTemplate.hpp"
