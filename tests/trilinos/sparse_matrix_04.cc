// ---------------------------------------------------------------------
//
// Copyright (C) 2011 - 2017 by the deal.II authors
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



// test TrilinosWrappers::SparseMatrix::reinit with a dealii::SparseMatrix
// without copying values

#include <deal.II/base/utilities.h>

#include <deal.II/lac/sparse_matrix.h>
#include <deal.II/lac/trilinos_sparse_matrix.h>
#include <deal.II/lac/trilinos_sparsity_pattern.h>

#include <iostream>

#include "../tests.h"


int
main(int argc, char **argv)
{
  Utilities::MPI::MPI_InitFinalize mpi_initialization(
    argc, argv, testing_max_num_threads());

  initlog();

  SparsityPattern sparsity(5, 5, 5);
  sparsity.add(1, 2);
  sparsity.add(2, 3);
  sparsity.add(3, 4);
  sparsity.add(4, 3);
  sparsity.compress();
  SparseMatrix<double> matrix(sparsity);
  {
    double value = 1;
    for (SparseMatrix<double>::iterator p = matrix.begin(); p != matrix.end();
         ++p, ++value)
      p->value() = value;
  }
  deallog << "Original:" << std::endl;
  matrix.print_formatted(deallog.get_file_stream());

  // now copy everything into a Trilinos matrix
  Epetra_Map map(TrilinosWrappers::types::int_type(5),
                 TrilinosWrappers::types::int_type(5),
                 0,
                 Utilities::Trilinos::comm_world());

  TrilinosWrappers::SparseMatrix tmatrix;
  tmatrix.reinit(map, map, matrix, 0, false);

  deallog << "Copy structure only:" << std::endl;
  tmatrix.print(deallog.get_file_stream());
}
