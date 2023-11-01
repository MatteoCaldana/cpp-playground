import matplotlib.pyplot as plt
import pandas as pd

if __name__ == "__main__":
    fig, axs = plt.subplots(1, 2)
    cmaps = [["jet", "jet"], ["Oranges", "Reds"], ["Greens", "Greys"]]
    for i, compile_lvl in enumerate(["release", "aggressive"]):
        for j, precision in enumerate(["single", "double"]):
            df = pd.read_csv(
                f"../data/{compile_lvl}.{precision}.csv",
            )
            for k, col in enumerate(df.columns):
                if col != "size":
                    y = df[col].apply(lambda x: float(x.split(";")[0]))
                    ye = df[col].apply(lambda x: float(x.split(";")[1]))
                    cmap = plt.get_cmap(cmaps[0][j])
                    axs[j].plot(
                        df["size"],
                        y,
                        label=f"{col}",
                        color=cmap(k / (len(df.columns) - 1)),
                        linestyle="-" if i else "--",
                    )
                    axs[j].legend(ncol=2)
                    axs[j].set_title(precision)
                    axs[j].set_yscale("log")
                    axs[j].set_xscale("log")
                    axs[j].grid("on")

    plt.show()
