#ifdef HAVE_MPI
    #define CHECK_TIME(str) {\
        time = timer_full.split();\
        timer_full.reset();\
        MPI_Barrier( MPI_COMM_WORLD );\
        time_dead = timer_full.split();\
        if( isMaster ) {\
            time_v[0] = time;\
            time_dead_v[0] = time_dead;\
            for( int i = 1; i < size; ++i) {\
                MPI_Recv( &time_v[i],      1, MPI_UNSIGNED_LONG_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);\
                MPI_Recv( &time_dead_v[i], 1, MPI_UNSIGNED_LONG_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);\
            }\
        } else {\
            MPI_Send( &time,      1, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD);\
            MPI_Send( &time_dead, 1, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD);\
        }\
        if( isMaster ) {\
            std::cout<<str<<"\n";\
            for(int i = 0; i < size; ++i)\
                std::cout<<"  rank: "<<i<<"\ttime: "<<((double)time_v[i])/rescale\
                         <<"\tdead: "<<((double)time_dead_v[i])/rescale<<"\n";\
            std::cout<<std::endl;\
        }\
        MPI_Barrier( MPI_COMM_WORLD );\
        timer_full.reset();\
    }
#else
    #define CHECK_TIME(str) {\
        time = timer_full.split();\
        timer_full.reset();\
        std::cout<<str<<"\ttime: "<<((double)time)/rescale<<"\n";\
        timer_full.reset();\
    }
#endif