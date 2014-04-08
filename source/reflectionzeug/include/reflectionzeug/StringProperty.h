
#pragma once

#include <vector>

#include <signalzeug/Signal.h>

#include <reflectionzeug/reflectionzeug.h>
#include <reflectionzeug/ValueProperty.h>

namespace reflectionzeug
{
    
/** \brief Part of the property hierarchy that manages a string and can have choices.
 */
class REFLECTIONZEUG_API StringProperty : public ValueProperty<std::string>
{
public:
    StringProperty(const std::string & value);
    
    StringProperty(const std::function<std::string ()> & getter,
                   const std::function<void(const std::string &)> & setter);
    
    template <class Object>
    StringProperty(Object & object, 
                   const std::string & (Object::*getter_pointer)() const,
                   void (Object::*setter_pointer)(const std::string &));
    
    template <class Object>
    StringProperty(Object & object, 
                   std::string (Object::*getter_pointer)() const,
                   void (Object::*setter_pointer)(const std::string &));

    template <class Object>
    StringProperty(Object & object, 
                   std::string (Object::*getter_pointer)() const,
                   void (Object::*setter_pointer)(std::string));
    
    virtual ~StringProperty() = 0;

    bool hasChoices() const;
    void addChoice(const std::string & string);

    const std::vector<std::string>  & choices() const;
    void setChoices(const std::vector<std::string> & choices);
    
    void clearChoices();

    virtual std::string toString() const;
    virtual bool fromString(const std::string & string);
    
    signalzeug::Signal<const std::vector<std::string> &> choicesChanged;
    
protected:
    std::vector<std::string> m_choices;
};

} // namespace reflectionzeug

#include "StringProperty.hpp"