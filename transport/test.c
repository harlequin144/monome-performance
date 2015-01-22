int quit(const char *path, const char *types, lo_arg ** argv,
		                    int argc, void *data, void *user_data)
{
	unsigned int ** run = (unsigned int **) user_data;
	*run = 0;
	return 0;
}



