#include "block.h"
#include "node.h"
#include <mpi.h>


using MPI::COMM_WORLD; using MPI::ANY_SOURCE; using MPI::ANY_TAG;
using MPI::INT; using MPI::CHAR; using MPI::BOOL;
using MPI::Status;

using namespace std;

// Variables de MPI
MPI_Datatype *MPI_BLOCK;

int main(int argc, char **argv) {

  // Inicializo MPI
  int status = MPI_Init(&argc, &argv);
  if (status != MPI_SUCCESS){
      fprintf(stderr, "Error de MPI al inicializar.\n");
      MPI_Abort(MPI_COMM_WORLD, status);
  }


  //Defino un nuevo tipo de datos de MPI para Block: MPI_BLOCK
  MPI_BLOCK = new MPI_Datatype;
  define_block_data_type_for_MPI(MPI_BLOCK);


  // Control del buffering: sin buffering
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  //Llama a la función que maneja cada nodo
  node();

  // Limpio MPI
  MPI_Finalize();
  delete MPI_BLOCK;

  return 0;
}
