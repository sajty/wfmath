// -*-C++-*-
// intersect.h (Shape intersection functions)
//
//  The WorldForge Project
//  Copyright (C) 2000, 2001  The WorldForge Project
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//  For information about WorldForge and its authors, please contact
//  the Worldforge Web Site at http://www.worldforge.org.
//

#ifndef WFMATH_INTERSECT_H
#define WFMATH_INTERSECT_H

#include <wfmath/vector.h>
#include <wfmath/point.h>
#include <wfmath/const.h>
#include <wfmath/intersect_decls.h>
#include <wfmath/axisbox.h>
#include <wfmath/ball.h>

namespace WF { namespace Math {

// Get the reversed order intersect functions (is this safe? FIXME)

template<class S1, class S2>
inline bool Intersect(const S1& s1, const S2& s2)
{
  return Intersect(s2, s1);
}

template<class S1, class S2>
inline bool IntersectProper(const S1& s1, const S2& s2)
{
  return IntersectProper(s2, s1);
}

// Point<>

template<const int dim>
inline bool Intersect(const Point<dim>& p1, const Point<dim>& p2)
{
  return p1 == p2;
}

template<const int dim>
inline bool IntersectProper(const Point<dim>& p1, const Point<dim>& p2)
{
  return false;
}

template<const int dim, class S>
inline bool Contains(const S& s, const Point<dim>& p)
{
  return Intersect(p, s);
}

template<const int dim>
inline bool Contains(const Point<dim>& p1, const Point<dim>& p2)
{
  return p1 == p2;
}

template<const int dim, class S>
inline bool ContainsProper(const Point<dim>& p, const S& s)
{
  return false;
}

template<const int dim, class S>
inline bool ContainsProper(const S& s, const Point<dim>& p)
{
  return IntersectProper(p, s);
}

template<const int dim>
inline bool ContainsProper(const Point<dim>& p1, const Point<dim>& p2)
{
  return false;
}

// AxisBox<>

template<const int dim>
bool Intersect(const AxisBox<dim>& b, const Point<dim>& p)
{
  for(int i = 0; i < dim; ++i)
    if(b.m_low[i] > p[i] || b.m_high[i] < p[i])
      return false;

  return true;
}

template<const int dim>
bool IntersectProper(const AxisBox<dim>& b, const Point<dim>& p)
{
  for(int i = 0; i < dim; ++i)
    if(b.m_low[i] >= p[i] || b.m_high[i] <= p[i])
      return false;

  return true;
}

template<const int dim>
inline bool Contains(const Point<dim>& p, const AxisBox<dim>& b)
{
  return p == b.m_low && p == b.m_high;
}

template<const int dim>
bool Intersect(const AxisBox<dim>& b1, const AxisBox<dim>& b2)
{
  for(int i = 0; i < dim; ++i)
    if(b1.m_low[i] > b2.m_high[i] || b1.m_high[i] < b2.m_low[i])
      return false;

  return true;
}

template<const int dim>
bool IntersectProper(const AxisBox<dim>& b1, const AxisBox<dim>& b2)
{
  for(int i = 0; i < dim; ++i)
    if(b1.m_low[i] >= b2.m_high[i] || b1.m_high[i] <= b2.m_low[i])
      return false;

  return true;
}

template<const int dim>
bool Contains(const AxisBox<dim>& outer, const AxisBox<dim>& inner)
{
  for(int i = 0; i < dim; ++i)
    if(inner.m_low[i] < outer.m_low[i] || inner.m_high[i] > outer.m_high[i])
      return false;

  return true;
}

template<const int dim>
bool ContainsProper(const AxisBox<dim>& outer, const AxisBox<dim>& inner)
{
  for(int i = 0; i < dim; ++i)
    if(inner.m_low[i] <= outer.m_low[i] || inner.m_high[i] >= outer.m_high[i])
      return false;

  return true;
}

// Ball<>

template<const int dim>
bool Intersect(const Ball<dim>& b, const Point<dim>& p)
{
  return SquaredDistance(b.m_center, p) <= b.m_radius * b.m_radius;
}

template<const int dim>
bool IntersectProper(const Ball<dim>& b, const Point<dim>& p)
{
  return SquaredDistance(b.m_center, p) < b.m_radius * b.m_radius;
}

template<const int dim>
inline bool Contains(const Point<dim>& p, const Ball<dim>& b)
{
  return b.m_radius == 0 && p == b.m_center;
}

template<const int dim>
bool Intersect(const Ball<dim>& b, const AxisBox<dim>& a)
{
  CoordType dist = 0;

  // Don't use FloatAdd(), only need values for comparison

  for(int i = 0; i < dim; ++i) {
    double dist_i;
    if(b.m_center[i] < a.m_low[i])
      dist_i = b.m_center[i] - a.m_low[i];
    else if(Shape<dim>::m_center[i] > a.m_high[i])
      dist_i = b.m_center[i] - a.m_high[i];
    else
      continue;
    dist+= dist_i * dist_i;
  }

  return dist <= b.m_radius * b.m_radius;
}

template<const int dim>
bool IntersectProper(const Ball<dim>& b, const AxisBox<dim>& a)
{
  CoordType dist = 0;

  // Don't use FloatAdd(), only need values for comparison

  for(int i = 0; i < dim; ++i) {
    double dist_i;
    if(b.m_center[i] < a.m_low[i])
      dist_i = b.m_center[i] - a.m_low[i];
    else if(Shape<dim>::m_center[i] > a.m_high[i])
      dist_i = b.m_center[i] - a.m_high[i];
    else
      continue;
    dist+= dist_i * dist_i;
  }

  return dist < b.m_radius * b.m_radius;
}

template<const int dim>
bool Contains(const Ball<dim>& b, const AxisBox<dim>& a)
{
  double sqr_dist = 0;

  for(int i = 0; i < (1 << dim); ++i) {
    double furthest = std::max(fabs(b.m_center[i] - a.m_low[i]),
			       fabs(b.m_center[i] - a.m_high[i]));
    sqr_dist += furthest * furthest;
  }

  return sqr_dist <= b.m_radius * b.m_radius;
}

template<const int dim>
bool ContainsProper(const Ball<dim>& b, const AxisBox<dim>& a)
{
  double sqr_dist = 0;

  for(int i = 0; i < (1 << dim); ++i) {
    double furthest = std::max(fabs(b.m_center[i] - a.m_low[i]),
			       fabs(b.m_center[i] - a.m_high[i]));
    sqr_dist += furthest * furthest;
  }

  return sqr_dist < b.m_radius * b.m_radius;
}

template<const int dim>
bool Contains(const AxisBox<dim>& a, const Ball<dim>& b)
{
  // Don't use FloatAdd(), only need values for comparison

  for(int i = 0; i < (1 << dim); ++i)
    if(b.m_center[i] - m_radius < b.lowerBound(i)
       || b.m_center[i] + m_radius > b.upperBound(i))
      return false;

  return true;
}

template<const int dim>
bool ContainsProper(const AxisBox<dim>& a, const Ball<dim>& b)
{
  // Don't use FloatAdd(), only need values for comparison

  for(int i = 0; i < (1 << dim); ++i)
    if(b.m_center[i] - m_radius <= b.lowerBound(i)
       || b.m_center[i] + m_radius >= b.upperBound(i))
      return false;

  return true;
}

template<const int dim>
bool Intersect(const Ball<dim>& b1, const Ball<dim>& b2)
{
  double sqr_dist = SquaredDistance(b1.m_center, b2.m_center);
  double rad_sum = b1.m_radius + b2.m_radius;

  return sqr_dist <= rad_sum * rad_sum;
}

template<const int dim>
bool IntersectProper(const Ball<dim>& b1, const Ball<dim>& b2)
{
  double sqr_dist = SquaredDistance(b1.m_center, b2.m_center);
  double rad_sum = b1.m_radius + b2.m_radius;

  return sqr_dist < rad_sum * rad_sum;
}

template<const int dim>
bool Contains(const Ball<dim>& outer, const Ball<dim>& inner)
{
  // Need FloatSubtract() so a ball will contain itself
  double rad_diff = FloatSubtract(outer.m_radius, inner.m_radius);

  if(rad_diff < 0)
    return false;

  double sqr_dist = SquaredDistance(outer.m_center, inner.m_center);

  return sqr_dist <= rad_diff * rad_diff;
}

template<const int dim>
bool ContainsProper(const Ball<dim>& outer, const Ball<dim>& inner)
{
  // Need FloatSubtract() so a ball will not contain itself properly
  double rad_diff = FloatSubtract(outer.m_radius, inner.m_radius);

  if(rad_diff <= 0)
    return false;

  double sqr_dist = SquaredDistance(outer.m_center, inner.m_center);

  return sqr_dist < rad_diff * rad_diff;
}

// Segment<>

template<const int dim>
bool Intersect(const Segment<dim>& s, const Point<dim>& p)
{
  // This is only true if p lies on the line between m_p1 and m_p2

  Vector<dim> v1 = s.m_p1 - p, v2 = s.m_p2 - p;

  double proj = Dot(v1, v2);

  if(proj > 0) // p is on the same side of both ends, not between them
    return false;

  // Check for colinearity
  return IsFloatEqual(proj * proj, v1.sqrMag() * v2.sqrMag());
}

template<const int dim>
bool IntersectProper(const Segment<dim>& s, const Point<dim>& p)
{
  // This is only true if p lies on the line between m_p1 and m_p2

  Vector<dim> v1 = s.m_p1 - p, v2 = s.m_p2 - p;

  double proj = Dot(v1, v2);

  if(proj >= 0) // p is on the same side of both ends, not between them
    return false;

  // Check for colinearity
  return IsFloatEqual(proj * proj, v1.sqrMag() * v2.sqrMag());
}

template<const int dim>
inline bool Contains(const Point<dim>& p, const Segment<dim>& s)
{
  return p == s.m_p1 && p == s.m_p2;
}

template<const int dim>
bool Intersect(const Segment<dim>& s, const AxisBox<dim>& b)
{
  // Use parametric coordinates on the line, where 0 is the location
  // of m_p1 and 1 is the location of m_p2

  // Find the parametric coordinates of the portion of the line
  // which lies betweens b.lowerBound(i) and b.upperBound(i) for
  // each i. Find the intersection of those segments and the
  // segment (0, 1), and check if it's nonzero.

  double min = 0, max = 1, low, high;

  for(int i = 0; i < dim; ++i) {
    double dist = FloatSubtract(s.m_p2[i], s.m_p1[i]);
    if(dist == 0) {
      if(s.m_p1[i] < b.m_low[i] || s.m_p1[i] > b.m_high[i])
        return false;
    }
    else {
      low = (b.m_low[i] - s.m_p1[i]) / dist;
      high = (b.m_high[i] - s.m_p1[i]) / dist;
      if(low > min)
        min = low;
      if(high < max)
        max = high;
    }
  }

  return min <= max;
}

template<const int dim>
bool IntersectProper(const Segment<dim>& s, const AxisBox<dim>& b)
{
  // Use parametric coordinates on the line, where 0 is the location
  // of m_p1 and 1 is the location of m_p2

  // Find the parametric coordinates of the portion of the line
  // which lies betweens b.lowerBound(i) and b.upperBound(i) for
  // each i. Find the intersection of those segments and the
  // segment (0, 1), and check if it's nonzero.

  double min = 0, max = 1, low, high;

  for(int i = 0; i < dim; ++i) {
    double dist = FloatSubtract(s.m_p2[i], s.m_p1[i]);
    if(dist == 0) {
      if(s.m_p1[i] <= b.m_low[i] || s.m_p1[i] >= b.m_high[i])
        return false;
    }
    else {
      low = (b.m_low[i] - s.m_p1[i]) / dist;
      high = (b.m_high[i] - s.m_p1[i]) / dist;
      if(low > min)
        min = low;
      if(high < max)
        max = high;
    }
  }

  return min < max;
}

template<const int dim>
bool Contains(const Segment<dim>& s, const AxisBox<dim>& b)
{
  // This is only possible for zero width or zero height box,
  // in which case we check for containment of the endpoints.

  bool got_difference = false;

  for(int i = 0; i < dim; ++i) {
    if(b.m_low[i] == b.m_high[i])
      continue;
    if(got_difference)
      return false;
    else // It's okay to be different on one axis
      got_difference = true;
  }

  return Contains(s, b.m_low) && (got_difference ? Contains(s, b.m_high) : true);
}

template<const int dim>
bool ContainsProper(const Segment<dim>& s, const AxisBox<dim>& b)
{
  // This is only possible for zero width or zero height box,
  // in which case we check for containment of the endpoints.

  bool got_difference = false;

  for(int i = 0; i < dim; ++i) {
    if(b.m_low[i] == b.m_high[i])
      continue;
    if(got_difference)
      return false;
    else // It's okay to be different on one axis
      got_difference = true;
  }

  return ContainsProper(s, b.m_low) && (got_difference ?
	 ContainsProper(s, b.m_high) : true);
}

template<const int dim>
inline bool Contains(const AxisBox<dim>& b, const Segment<dim>& s)
{
  return Contains(b, s.m_p1) && Contains(b, s.m_p2);
}

template<const int dim>
inline bool ContainsProper(const AxisBox<dim>& b, const Segment<dim>& s)
{
  return ContainsProper(b, s.m_p1) && ContainsProper(b, s.m_p2);
}

template<const int dim>
bool Intersect(const Segment<dim>& s, const Ball<dim>& b)
{
  Vector<dim> line = s.m_p2 - s.m_p1, offset = b.m_center - s.m_p1;

  // First, see if the closest point on the line to the center of
  // the ball lies inside the segment

  CoordType proj = Dot(line, offset), lineSqrMag = line.sqrMag();

  if(proj < 0 || proj > lineSqrMag) {
    // The nearest point on the line is outside the segment. Intersection reduces
    // to checking the endpoints.

    return Intersect(b, s.m_p1) || Intersect(b, s.m_p2);
  }

  Vector<dim> perp_part = offset - line * proj / lineSqrMag;

  return perp_part.sqrMag() <= b.m_radius * b.m_radius;
}

template<const int dim>
bool IntersectProper(const Segment<dim>& s, const Ball<dim>& b)
{
  Vector<dim> line = s.m_p2 - s.m_p1, offset = b.m_center - s.m_p1;

  // First, see if the closest point on the line to the center of
  // the ball lies inside the segment

  CoordType proj = Dot(line, offset), lineSqrMag = line.sqrMag();

  if(proj < 0 || proj > lineSqrMag) {
    // The nearest point on the line is outside the segment. Intersection reduces
    // to checking the endpoints.

    return IntersectProper(b, s.m_p1) || IntersectProper(b, s.m_p2);
  }

  Vector<dim> perp_part = offset - line * proj / lineSqrMag;

  return perp_part.sqrMag() < b.m_radius * b.m_radius;
}

template<const int dim>
inline bool Contains(const Ball<dim>& b, const Segment<dim>& s)
{
  return Contains(b, s.m_p1) && Contains(b, s.m_p2);
}

template<const int dim>
inline bool ContainsProper(const Ball<dim>& b, const Segment<dim>& s)
{
  return ContainsProper(b, s.m_p1) && ContainsProper(b, s.m_p2);
}

template<const int dim>
inline bool Contains(const Segment<dim>& s, const Ball<dim>& b)
{
  return b.m_radius == 0 && Contains(s, b.m_center);
}

template<const int dim>
inline bool ContainsProper(const Segment<dim>& s, const Ball<dim>& b)
{
  return b.m_radius == 0 && ContainsProper(s, b.m_center);
}

template<const int dim>
bool Intersect(const Segment<dim>& s1, const Segment<dim>& s2)
{
  // Check that the lines that contain the segments intersect, and then check
  // that the intersection point lies within the segments

  Vector<dim> v1 = s1.m_p2 - s1.m_p1, v2 = s2.m_p2 - s2.m_p1,
	      deltav = s2.m_p1 - s1.m_p1;

  CoordType v1sqr = v1.sqrMag(), v2sqr = v2.sqrMag(), deltavsqr = deltav.sqrMag();
  CoordType proj12 = Dot(v1, v2), proj1delta = Dot(v1, deltav),
	    proj2delta = Dot(v2, deltav);

  CoordType denom = FloatSubtract(v1sqr * v2sqr, proj12 * proj12);

  if(!IsFloatEqual(FloatAdd(v2sqr * proj1delta * proj1delta,
			    v1sqr * proj2delta * proj2delta),
		   FloatAdd(2 * proj12 * proj1delta * proj2delta,
			    deltavsqr * denom)))
    return false; // Lines don't intersect

  if(denom > 0) {
    // Find the location of the intersection point in parametric coordinates,
    // where one end of the segment is at zero and the other at one

    CoordType coord1 = FloatSubtract(v2sqr * proj1delta, proj12 * proj2delta) / denom;
    CoordType coord2 = -FloatSubtract(v1sqr * proj2delta, proj12 * proj1delta) / denom;

    return coord1 >= 0 && coord1 <= 1 && coord2 >= 0 && coord2 <= 1;
  }
  else {
    // Colinear segments, see if one contains an endpoint of the other
    return Contains(s1, s2.m_p1) || Contains(s1, s2.m_p2)
	|| Contains(s2, s1.m_p1) || Contains(s2, s1.m_p2);
  }
}

template<const int dim>
bool IntersectProper(const Segment<dim>& s1, const Segment<dim>& s2)
{
  // Check that the lines that contain the segments intersect, and then check
  // that the intersection point lies within the segments

  Vector<dim> v1 = s1.m_p2 - s1.m_p1, v2 = s2.m_p2 - s2.m_p1,
	      deltav = s2.m_p1 - s1.m_p1;

  CoordType v1sqr = v1.sqrMag(), v2sqr = v2.sqrMag(), deltavsqr = deltav.sqrMag();
  CoordType proj12 = Dot(v1, v2), proj1delta = Dot(v1, deltav),
	    proj2delta = Dot(v2, deltav);

  CoordType denom = FloatSubtract(v1sqr * v2sqr, proj12 * proj12);

  if(!IsFloatEqual(FloatAdd(v2sqr * proj1delta * proj1delta,
			    v1sqr * proj2delta * proj2delta),
		   FloatAdd(2 * proj12 * proj1delta * proj2delta,
			    deltavsqr * denom)))
    return false; // Lines don't intersect

  if(denom > 0) {
    // Find the location of the intersection point in parametric coordinates,
    // where one end of the segment is at zero and the other at one

    CoordType coord1 = FloatSubtract(v2sqr * proj1delta, proj12 * proj2delta) / denom;
    CoordType coord2 = -FloatSubtract(v1sqr * proj2delta, proj12 * proj1delta) / denom;

    return coord1 > 0 && coord1 < 1 && coord2 > 0 && coord2 < 1;
  }
  else {
    // Colinear segments, see if one contains an endpoint of the other
    return ContainsProper(s1, s2.m_p1) || ContainsProper(s1, s2.m_p2)
	|| ContainsProper(s2, s1.m_p1) || ContainsProper(s2, s1.m_p2);
  }
}

template<const int dim>
inline bool Contains(const Segment<dim>& s1, const Segment<dim>& s2)
{
  return Contains(s1, s2.m_p1) && Contains(s1, s2.m_p2);
}

template<const int dim>
inline bool ContainsProper(const Segment<dim>& s1, const Segment<dim>& s2)
{
  return ContainsProper(s1, s2.m_p1) && ContainsProper(s1, s2.m_p2);
}

// RotBox<>

template<const int dim>
bool Intersect(const RotBox<dim>& r, const Point<dim>& p)
{
  // Rotate the point into the internal coordinate system of the box

  Vector<dim> shift = InvProd(r.m_orient, p - r.m_corner0);

  for(int i = 0; i < dim; ++i) {
    if(r.m_size[i] < 0) {
      if(shift[i] < r.m_size[i] || shift[i] > 0)
        return false;
    }
    else {
      if(shift[i] > r.m_size[i] || shift[i] < 0)
        return false;
    }
  }

  return true;
}

template<const int dim>
bool IntersectProper(const RotBox<dim>& r, const Point<dim>& p)
{
  // Rotate the point into the internal coordinate system of the box

  Vector<dim> shift = InvProd(r.m_orient, p - r.m_corner0);

  for(int i = 0; i < dim; ++i) {
    if(r.m_size[i] < 0) {
      if(shift[i] <= r.m_size[i] || shift[i] >= 0)
        return false;
    }
    else {
      if(shift[i] >= r.m_size[i] || shift[i] <= 0)
        return false;
    }
  }

  return true;
}

template<const int dim>
bool Contains(const Point<dim>& p, const RotBox<dim>& r)
{
  for(int i = 0; i < dim; ++i)
    if(r.m_size != 0)
      return false;

  return p == r.m_corner0;
}

// FIXME Intersect, Contains functions for RotBox<> and AxisBox<>

template<const int dim>
bool Contains(const AxisBox<dim>& b, const RotBox<dim>& r)
{
  return Contains(b, r.boundingBox());
}

template<const int dim>
bool ContainsProper(const AxisBox<dim>& b, const RotBox<dim>& r)
{
  return ContainsProper(b, r.boundingBox());
}

// FIXME RotBox intersect functions with Ball, Segment, itself

}} // namespace WF::Math

#endif  // WFMATH_INTERSECT_H
