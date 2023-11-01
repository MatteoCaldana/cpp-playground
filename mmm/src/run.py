import subprocess

if __name__ == "__main__":
    for compile_lvl in ["release", "aggressive"]:
        for precision in ["single", "double"]:
            print("Running", compile_lvl, precision)
            subprocess.run(
                f"./{compile_lvl}.out --mode benchmark --repetitions 2 --max_size 11 --precision {precision} > ../data/{compile_lvl}.{precision}.csv",
                shell=True,
            )
