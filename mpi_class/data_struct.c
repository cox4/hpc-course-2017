#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

struct Particle{
  float x;
  float y;
  float mass;
  float fx;
  float fy;
};

int main(int argc, char* argv[]) {
  int myrank, numprocs;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

  // Define an array of particles.
  int num_parts_to_test = 10;
  struct Particle particles[num_parts_to_test];

  MPI_Datatype ParticleStruct, ParticleType;

  // This defines the mapping between the struct and the MPI
  // Datatypes.
  MPI_Datatype types[5] = {MPI_DOUBLE,
			   MPI_DOUBLE,
			   MPI_DOUBLE, 
			   MPI_DOUBLE,
			   MPI_DOUBLE};

  // This is the length of each element, all 1 in this case.
  int          blocklens[5] = {1, 1, 1, 1, 1}; 

  // This is the offsets defined in the struct.
  MPI_Aint     displacements[5];

  // Get the displacements using the particles array. Note that the
  // first displacment would be the pointer value of &particles. Then each
  // subsequent element in the struct gives us the displacement.
  MPI_Get_address(particles,          displacements); 
  MPI_Get_address(&particles[0].y,    displacements+1); 
  MPI_Get_address(&particles[0].mass, displacements+2); 
  MPI_Get_address(&particles[0].fx,   displacements+3); 
  MPI_Get_address(&particles[0].fy,   displacements+4); 

  // At this point, for instance:
  // disp[0] = 1001 <- some random location in memory where particles is allocated.
  // disp[1] = 1005
  // disp[2] = 1009
  // disp[3] = 1013
  // disp[4] = 1017

  // Now convert these addresses into actual displacements.
  MPI_Aint     base;
  base = displacements[0];
  int i;
  for (i=0; i < 5; i++) {
    displacements[i] = displacements[i] - base;
  }

  // At this point we should have displacements:
  // disp[0] = 0
  // disp[1] = 4
  // disp[2] = 8
  // disp[3] = 12
  // disp[4] = 16

  // Now we create the particle struct.
  MPI_Type_create_struct(5, blocklens, displacements, types, &ParticleStruct);

  // Once we have that we can get the size of a single partcle.
  MPI_Aint sizeOfParticle;
  MPI_Get_address(particles+1, &sizeOfParticle);
  sizeOfParticle = sizeOfParticle - base;

  // We can then create an optimized version of this type.
  MPI_Type_create_resized(ParticleStruct, 0, sizeOfParticle, &ParticleType);

  // Lastly, we commit this type to MPI for use!
  MPI_Type_commit(&ParticleType);


  // Example usage of this new data type, let rank 0 fill with test
  // data then broadcast that to every other rank.
  if (myrank == 0) {
    for (i = 0; i < num_parts_to_test; i++) {
      particles[i].x = i * 10;
      particles[i].y = i * 10 + 2;
      particles[i].mass = i + 1;
      particles[i].fx = (i+1 * 100) / 1000.;
      particles[i].fy = (i+1 * 10) / 230.;
    }
  }

  MPI_Bcast(particles, num_parts_to_test, ParticleType, 0, MPI_COMM_WORLD);

  for (i = 0; i < num_parts_to_test; i++) {
    printf("Rank %d - particles[%d] <%f, %f, %f, %f, %f>\n", 
	   myrank,
	   i,
	   particles[i].x, 
	   particles[i].y, 
	   particles[i].mass,
	   particles[i].fx,
	   particles[i].fy);
  }

  MPI_Finalize();
  return 0;
}
