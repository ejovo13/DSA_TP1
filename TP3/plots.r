library(tidyverse)
library(ggplot2)
library(tikzDevice)

df <- as_tibble(read.csv("average_data.csv"))

df_long <- df |> pivot_longer(c(u_height, u_deq), names_to = "stat", values_to = "value")

tikzDevice::tikz(file = "average_height.tex", width = 7, height = 3)

df_long |> ggplot(aes(log2n, value)) + geom_point(aes(color = stat)) +
    labs(y = "", col = "Statistique") +
    scale_color_hue(labels = c("$\\mu_f$", "$\\mu_{hauteur}$"))

dev.off()
