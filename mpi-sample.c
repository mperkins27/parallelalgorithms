for (int i=nTasks*4; i < C_MAX_ARRAY_SIZE; i += (C_MAX_ARRAY_SIZE >> 4))
{
	int numOfElems = i/nTasks;
	int startIndex = numOfElems * rank;
	double startTime = MPI_Wtime();
	for (int j=startIndex; j < startIndex + numOfElems; j++)
	{
		g_testArray[j] = j*j - sqrt((double)j);	// do a simple computation
	}
	// each thread needs to send results to thread 0
	if (rank == 0)
	{
		// The master thread will need to receive all computations from all other threads.
		MPI_Status status;
		for (int j=1; j < nTasks; j++)
		{
			MPI_Recv(&g_testArray[j*numOfElems], numOfElems, MPI_INT, j,3, MPI_COMM_WORLD, &status);
		}
	}
	else
	{
		MPI_Send(&g_testArray[startIndex], numOfElems, MPI_INT, 0, 3, MPI_COMM_WORLD);
	}
	double endTime = MPI_Wtime();
	if (rank == 0) printf("i = %d    time = %f\n", i, (float)(endTime - startTime));
}