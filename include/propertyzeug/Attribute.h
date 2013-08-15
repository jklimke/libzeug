
#pragma once

#include <propertyzeug/propertyzeug.h>
#include "BaseAttribute.h"
#include "LimitAttribute.h"
#include "StringAttribute.h"

#include "Color.h"

namespace propertyzeug {

template <typename Type>
class Attribute;

template <>
class Attribute<bool> : public BaseAttribute<bool>
{
public:
    Attribute(const std::string & name, const std::string & title, const bool & value)
    :   BaseAttribute<bool>(name, title, value) {};

    Attribute(const std::string & name, const std::string & title, 
              const std::function<const bool & ()> & getter,
              const std::function<void(const bool &)> & setter)
    :   BaseAttribute<bool>(name, title, getter, setter) {};
    
    template <class Object>
    Attribute(const std::string & name, const std::string & title,
              Object & object, const bool & (Object::*getter_pointer)() const,
              void (Object::*setter_pointer)(const bool &))
    :   BaseAttribute<bool>(name, title, object, getter_pointer, setter_pointer) {};
    
    template <class Object>
    Attribute(const std::string & name, const std::string & title,
              Object & object, bool (Object::*getter_pointer)() const,
              void (Object::*setter_pointer)(bool))
    :   BaseAttribute<bool>(name, title, object, getter_pointer, setter_pointer) {};
};
    
template <>
class Attribute<Color> : public BaseAttribute<Color>
{
public:
    Attribute(const std::string & name, const std::string & title, const Color & value)
    :   BaseAttribute<Color>(name, title, value) {};
    
    Attribute(const std::string & name, const std::string & title,
              const std::function<const Color & ()> & getter,
              const std::function<void(const Color &)> & setter)
    :   BaseAttribute<Color>(name, title, getter, setter) {};
    
    template <class Object>
    Attribute(const std::string & name, const std::string & title,
              Object & object, const Color & (Object::*getter_pointer)() const,
              void (Object::*setter_pointer)(const Color &))
    :   BaseAttribute<Color>(name, title, object, getter_pointer, setter_pointer) {};
    
    template <class Object>
    Attribute(const std::string & name, const std::string & title,
              Object & object, Color (Object::*getter_pointer)() const,
              void (Object::*setter_pointer)(Color))
    :   BaseAttribute<Color>(name, title, object, getter_pointer, setter_pointer) {};
};

template <>
class Attribute<std::string> : public StringAttribute
{
public:
    Attribute(const std::string & name, const std::string & title, const std::string & value)
    :   StringAttribute(name, title, value) {};
    
    Attribute(const std::string & name, const std::string & title,
                  const std::function<const std::string & ()> & getter,
                  const std::function<void(const std::string &)> & setter)
    :   StringAttribute(name, title, getter, setter) {};
    
    template <class Object>
    Attribute(const std::string & name, const std::string & title,
                  Object & object, const std::string & (Object::*getter_pointer)() const,
                  void (Object::*setter_pointer)(const std::string &))
    :   StringAttribute(name, title, object, getter_pointer, setter_pointer) {};   
    
    template <class Object>
    Attribute(const std::string & name, const std::string & title,
                  Object & object, std::string (Object::*getter_pointer)() const,
                  void (Object::*setter_pointer)(std::string))
    :   StringAttribute(name, title, object, getter_pointer, setter_pointer) {};   
};

template <>
class Attribute<int> : public BaseAttribute<int>
{
public:
    Attribute(std::string name, std::string title, int value) 
    : BaseAttribute<int>(name, title, value) {};
    
    Attribute(std::string name, std::string title, 
              std::function<const int & ()> getter,
              std::function<void(const int &)> setter)
    : BaseAttribute<int>(name, title, getter, setter) {};
};

} // namespace