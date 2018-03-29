#pragma once

#include "Intersection.h"
#include "Segment.h"

class Primitive {
public:
	virtual bool TestSegment(const Segment &s, Intersection &i)=0;
};