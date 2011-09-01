#include "CGAL_Nef_polyhedron.h"
#include "cgal.h"
#include "polyset.h"
#include <CGAL/minkowski_sum_3.h>

CGAL_Nef_polyhedron& CGAL_Nef_polyhedron::operator+=(const CGAL_Nef_polyhedron &other)
{
	if (other.dim == 2) {
		(*this->p2) += (*other.p2);
		this->dim = 2;
	}
	if (other.dim == 3) {
		(*this->p3) += (*other.p3);
		this->dim = 3;
	}
	return *this;
}

CGAL_Nef_polyhedron& CGAL_Nef_polyhedron::operator*=(const CGAL_Nef_polyhedron &other)
{
	if (other.dim == 2) {
		(*this->p2) *= (*other.p2);
		this->dim = 2;
	}
	if (other.dim == 3) {
		(*this->p3) *= (*other.p3);
		this->dim = 3;
	}
	return *this;
}

CGAL_Nef_polyhedron& CGAL_Nef_polyhedron::operator-=(const CGAL_Nef_polyhedron &other)
{
	if (other.dim == 2) {
		(*this->p2) -= (*other.p2);
		this->dim = 2;
	}
	if (other.dim == 3) {
		(*this->p3) -= (*other.p3);
		this->dim = 3;
	}
	return *this;
}

int CGAL_Nef_polyhedron::weight() const
{
	if (dim == 2)
		return p2->explorer().number_of_vertices();
	if (dim == 3)
		return p3->number_of_vertices();
	return 0;
}

/*!
	Creates a new PolySet and initializes it with the data from this polyhedron

	This method is not const since convert_to_Polyhedron() wasn't const
  in earlier versions of CGAL.
*/
PolySet *CGAL_Nef_polyhedron::convertToPolyset()
{
	PolySet *ps = new PolySet();
	CGAL_Polyhedron P;
	this->p3->convert_to_Polyhedron(P);

	typedef CGAL_Polyhedron::Vertex                                 Vertex;
	typedef CGAL_Polyhedron::Vertex_const_iterator                  VCI;
	typedef CGAL_Polyhedron::Facet_const_iterator                   FCI;
	typedef CGAL_Polyhedron::Halfedge_around_facet_const_circulator HFCC;

	for (FCI fi = P.facets_begin(); fi != P.facets_end(); ++fi) {
		HFCC hc = fi->facet_begin();
		HFCC hc_end = hc;
		Vertex v1, v2, v3;
		v1 = *VCI((hc++)->vertex());
		v3 = *VCI((hc++)->vertex());
		do {
			v2 = v3;
			v3 = *VCI((hc++)->vertex());
			double x1 = CGAL::to_double(v1.point().x());
			double y1 = CGAL::to_double(v1.point().y());
			double z1 = CGAL::to_double(v1.point().z());
			double x2 = CGAL::to_double(v2.point().x());
			double y2 = CGAL::to_double(v2.point().y());
			double z2 = CGAL::to_double(v2.point().z());
			double x3 = CGAL::to_double(v3.point().x());
			double y3 = CGAL::to_double(v3.point().y());
			double z3 = CGAL::to_double(v3.point().z());
			ps->append_poly();
			ps->append_vertex(x1, y1, z1);
			ps->append_vertex(x2, y2, z2);
			ps->append_vertex(x3, y3, z3);
		} while (hc != hc_end);
	}
	return ps;
}

extern CGAL_Nef_polyhedron2 minkowski2(const CGAL_Nef_polyhedron2 &a, const CGAL_Nef_polyhedron2 &b);

CGAL_Nef_polyhedron &CGAL_Nef_polyhedron::minkowski(const CGAL_Nef_polyhedron &other)
{
	if (other.dim == 2) {
		(*this->p2) = minkowski2(*this->p2, *other.p2);
		this->dim = 2;
	}
	if (other.dim == 3) {
		(*this->p3) = CGAL::minkowski_sum_3(*this->p3, *other.p3);
		this->dim = 3;
	}
	return *this;
}