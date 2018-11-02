// ---------------------------------------------------------------------
//
// Copyright (C) 2002 - 2018 by the deal.II authors
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


// read in a Triangulation<2,3> from a mesh file writtent in the UCD format

// the meshe used here were generated by Andrea Mola

#include <deal.II/dofs/dof_handler.h>

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_in.h>
#include <deal.II/grid/grid_out.h>
#include <deal.II/grid/manifold_lib.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>

#include <string>

#include "../tests.h"


int
main()
{
  initlog();
  std::string   name(std::string(SOURCE_DIR "/grids/kcs_initial.inp"));
  std::ifstream in;
  in.open(name.c_str());

  Triangulation<2, 3> tria;
  GridIn<2, 3>        gi;
  gi.attach_triangulation(tria);
  gi.read_ucd(in, true);
  deallog << '\t' << tria.n_vertices() << '\t' << tria.n_cells() << std::endl;


  deallog << "Material IDs: " << std::endl;
  Triangulation<2, 3>::active_cell_iterator cell = tria.begin_active();
  Triangulation<2, 3>::active_cell_iterator endc = tria.end();
  for (; cell != endc; ++cell)
    {
      deallog << cell << "  " << int(cell->material_id()) << std::endl;
    }

  deallog << "Manifold IDs: " << std::endl;
  cell = tria.begin_active();
  for (; cell != endc; ++cell)
    {
      for (unsigned int f = 0; f < GeometryInfo<2>::faces_per_cell; ++f)
        if (cell->face(f)->manifold_id() != numbers::flat_manifold_id)
          deallog << cell << " (" << f << ")  "
                  << int(cell->face(f)->manifold_id()) << std::endl;
    }
}
