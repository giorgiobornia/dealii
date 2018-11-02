// ---------------------------------------------------------------------
//
// Copyright (C) 2003 - 2017 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE.md at
// the top level directory of deal.II.
//
// ---------------------------------------------------------------------


// test that the grid generated by GridGenerator::hyper_ball<3> has
// the right number of external faces. this was not always the case,
// due to a bug in the 3d grid reordering code that didn't find a
// valid reordering, though we know that there is one. when not
// calling the grid reordering, the triangulation can generate a mesh,
// but it will have internal faces with the "external" 255 marker,
// leading to more external faces than one originally wanted


#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>

#include "../tests.h"


int
main()
{
  initlog();
  deallog << std::setprecision(2);

  // generate a hyperball in 3d
  Triangulation<3> tria;
  GridGenerator::hyper_ball(tria, Point<3>(), std::sqrt(3.));

  // output all faces. there should
  // be 6 external ones, but there
  // used to be a bug in the
  // triangulation code that
  // generated more than that, with
  // some of them internal to the
  // ball
  unsigned int external_faces = 0;
  for (Triangulation<3>::face_iterator face = tria.begin_face();
       face != tria.end_face();
       ++face)
    {
      deallog << face << "   " << face->boundary_id() << "  "
              << face->vertex_index(0) << " <" << face->vertex(0) << '>'
              << std::endl
              << "           " << face->vertex_index(1) << "  <"
              << face->vertex(1) << '>' << std::endl
              << "           " << face->vertex_index(2) << "  <"
              << face->vertex(2) << '>' << std::endl
              << "           " << face->vertex_index(3) << "  <"
              << face->vertex(3) << '>' << std::endl;
      if (face->at_boundary())
        ++external_faces;
    }

  deallog << "External faces: " << external_faces << std::endl;

  Assert(external_faces == 6, ExcInternalError());

  return 0;
}
