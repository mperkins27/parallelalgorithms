from mpi4py import MPI

comm = MPI.COMM_WORLD
world_rank = comm.Get_rank()
world_size = comm.Get_size()
processor_name = MPI.Get_processor_name()

print("Hello, world! From processor " + str(processor_name) + ", rank " + str(world_rank) + " out of " + str(world_size) + " processors.")

