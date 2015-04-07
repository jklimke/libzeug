
#pragma once


#include <reflectionzeug/new/TypedColor.h>


namespace reflectionzeug
{


template <typename... Args>
TypedColor::TypedColor(Args&&... args)
: AbstractTyped<Color>(std::forward<Args>(args)...)
{
}

TypedColor::~TypedColor()
{
}

Color TypedColor::toColor() const
{
	return this->getValue();
}

bool TypedColor::fromColor(const Color & color)
{
    this->setValue(color);
    return true;
}


} // namespace reflectionzeug
