library(tidyverse)
library(magrittr)
library(ggthemes)
library(scales)
library(viridis)
library(wesanderson) # For more color palettes
#library(tikzDevice)


# Make some really cool plots and write them to .tex files!!!


# Add logarithmic data to a dataframe for complexity plotting
stack_logs <- function(df) {

  df %<>%
    mutate(N_sq = N * N) %<>%
    mutate(N_log_N = N * log2(N)) %>%
    mutate(TWON_log_N = 2 * N_log_N) %>%
    mutate(HALFN_log_N = 1/2 * N_log_N) %>%
    mutate(ONE_TENTH_log_N = 0.1 * N_log_N)

  return(df)
}

log_plot <- function(df, field, title="Log plot", subtitle="") {
  p <- ggplot(df, aes(N)) +
    geom_line(aes(N, N_log_N), color="blue") +
    geom_line(aes(N, TWON_log_N), color="red") +
    geom_line(aes(N, field)) +
    geom_line(aes(N, HALFN_log_N), color ="green") +
    geom_line(aes(N, ONE_TENTH_log_N), color = "violet") +
    ggtitle(title, subtitle=subtitle)

  p

}

prefix = "/home/ejovo/MAIN/S6/DSA/TP/TP1/"

add_prefix <- function(base) {
  paste(prefix, base, sep="")
}

fbis   <- add_prefix("p_Bis.csv")
fa     <- add_prefix("p_Hoare.csv")
f      <- add_prefix("p_Lomuto.csv")
fs     <- add_prefix("p_Sedgewick.csv")

create_plots <- function(file_name, subtitle="") {

    df <- as_tibble(read.csv(file_name))
    df %<>% stack_logs()

    p_ech <- log_plot(df, df$avg_ech, title="Average Exchanges", subtitle=subtitle)
    p_cmp <- log_plot(df, df$avg_cmp, title="Average Comparisons", subtitle=subtitle)
    print(p_ech)
    print(p_cmp)

    # return(c(p_ech, p_cmp))
}

create_plots(fbis, "Bis")
create_plots(fa, "Parti A")
create_plots(f, "Lumuto")
create_plots(fs, "Sedgewick")

# Merge them all!
df_bis <- as_tibble(read.csv(fbis))
df_a   <- as_tibble(read.csv(fa))
df_lum <- as_tibble(read.csv(f))
df_sed <- as_tibble(read.csv(fs))

df_all <- df_bis %>%
            rename(ucmp.bis = avg_cmp,
                   uech.bis = avg_ech) |>
                #    uech.bis = avg_ech,
            mutate(ucmp.a = df_a$avg_cmp,
                   uech.a = df_a$avg_ech) |>
            mutate(ucmp.lum = df_lum$avg_cmp,
                   uech.lum = df_lum$avg_ech) |>
            mutate(ucmp.sed = df_sed$avg_cmp,
                   uech.sed = df_sed$avg_ech) |>
            mutate(usum.bis = ucmp.bis + uech.bis,
                   usum.a = ucmp.a + uech.a,
                   usum.lum = ucmp.lum + uech.lum,
                   usum.sed = ucmp.sed + uech.sed)

df_all.ucmp <- df_all |>
                pivot_longer(c(ucmp.a, ucmp.bis, ucmp.lum, ucmp.sed), names_to = "partition_type", values_to = "ucmp") |>
                mutate(part = str_replace(partition_type, "ucmp.", "")) |>
                select(N, ucmp, part)

df_all.uech <- df_all |>
                pivot_longer(c(uech.a, uech.bis, uech.lum, uech.sed), names_to = "partition_type", values_to = "uech") |>
                mutate(part = str_replace(partition_type, "uech.", "")) |>
                select(N, uech, part)

df_all.usum <- df_all |>
                pivot_longer(c(usum.a, usum.bis, usum.lum, usum.sed), names_to = "partition_type", values_to = "usum") |>
                mutate(part = str_replace(partition_type, "usum.", "")) |>
                select(N, usum, part)

# This is the distilled data that we need.....
df_all.mutant <- df_all.ucmp |>
                mutate(uech = df_all.uech$uech,
                       usum = df_all.usum$usum)


# Plot all of the average comparisons
df_all.ucmp |>
    ggplot(aes(N, ucmp, color = part)) +
    geom_line() +
    labs(title="Average comparisons", subtitle="How Partition scheme affects quicksort", col="Partition Scheme") +
    ylab("$\\mu$ comparaisons") +
    xlab("$n$") +
    scale_color_hue(labels = c("Parti A", "Bis", "Lomuto", "Sedgewick"))

# Plot the average exchanges
df_all.uech |>
    ggplot(aes(N, uech, color = part)) +
    geom_line() +
    labs(title="Average exchanges", subtitle="How Partition scheme affects quicksort", col="Partition Scheme") +
    ylab("$\\mu$ exchanges") +
    xlab("$n$") +
    scale_color_hue(labels = c("Parti A", "Bis", "Lomuto", "Sedgewick"))

df_all.usum |>
    ggplot(aes(N, usum, color = part)) +
    geom_line() +
    labs(title="Average sum of exchanges and comparisons", subtitle="How Partition scheme affects quicksort", col="Partition Scheme") +
    ylab("$\\mu_k + \\mu_x$ ") +
    xlab("$n$") +
    scale_color_hue(labels = c("Parti A", "Bis", "Lomuto", "Sedgewick"))

df_all.mutant %<>% select(N, part, ucmp, uech, usum) %<>%
    pivot_longer(c(ucmp, uech, usum), names_to = "stat", values_to = "value")

# New facet labels for stats names
stats.labs <- c("$\\mu_k$", "$\\mu_x$", "$\\mu_k + \\mu_x$")
names(stats.labs) <- c("ucmp", "uech", "usum")


my_palette <- c("#000000", "#5800FF", "#E900FF", "#FFC600")
my_palette <- c("#0F2C67", "#CD1818", "#F3950D", "#F4E185")
my_palette <- c("#D92027", "#FF9234", "#FFCD3C", "#35D0BA")

stats_plot <- ggplot(df_all.mutant, aes(N, value, color = part)) +
    geom_line() +
    facet_wrap(~stat, labeller = labeller(stat = stats.labs)) +
    # facet_wrap(~stat) +
    labs(title="Complexity Statistics", col="Partition Scheme") +
    xlab("Taille du tableau, $n$") +
    ylab("") +
    # theme_solarized_2(light = FALSE) +
#   theme(text = element_text(family = "DM Sans Medium", colour = "#EEEEEE"),
#     title = element_text(color = "#EEEEEE"),
#     axis.title.x = element_blank(),
#     panel.background = element_rect(fill = NA),
#     plot.background = element_rect(fill = "#111111"),
#     panel.grid.major = element_blank(),
#     panel.grid.minor = element_blank(),
#     legend.background = element_blank(),
#     legend.key = element_blank(),
#     legend.position = "bottom",
#     plot.title = element_text(hjust = 0.5)) +
    scale_color_hue(labels = c("Partie A", "Partie B", "Lumoto", "Sedgewick")) +
    #scale_color_brewer(palette = "Pastel1")
    #scale_color_brewer(palette = "GrandBudapest")
    #scale_color_manual(values = wes_palette(n = 4, "BottleRocket2"))
    #scale_color_manual(values = )
    #scale_color_manual(values = inferno(5))
    #scale_color_manual(values = rainbow(6))
    scale_color_manual(values = my_palette)
    # scale_color_viridis(discrete = TRUE, option="D")


stats_plot

# Let's crunch some graphs out for partition_bis
df_bis.all <- df_all.mutant |> filter(part == "bis") |>
  mutate(N_log_N = N * log2(N))

df_bis.ucmp <- df_bis.all |> filter(stat == "ucmp")
df_bis.uech <- df_bis.all |> filter(stat == "uech")
df_bis.usum <- df_bis.all |> filter(stat == "usum")

df_bis.all_log <- stack_logs(df_bis.all)

df_bis.all_log |> ggplot(aes(N_log_N, value, color = stat)) + geom_line(size=1.2) +
  geom_line(aes(N_log_N, N_log_N), linetype="dashed", color = "black", size=1.2)

# Let's plot ucmp, usum, uech along with n^2

# Plot ucmp and n
# tikzDevice::tikz(file = "./df_bis_ucmp.tex", width = 7, height = 3)

df_bis.ucmp |>
    ggplot(aes(N, value, color = stat)) +
    geom_line() +
    labs (
        title = "Moyenne des comparaisons",
        subtitle = "Schéma partitionBis appliqué aux tableaux de taille $n$",
        x = "$n$",
        y = "",
        col = ""
    ) +
    scale_color_hue(labels = c("$\\mu_{cmp}$"))

# dev.off()
# dev.on()

latex_labels <- c("$2n \\log n$", "$\\mu_{cmp}$", "$n \\log n$")
labels_rearranged <- c("TWON_log_N", "value", "N_log_N")

df_bis.ucmp_log <- stack_logs(df_bis.ucmp) |>
    # pivot_longer(c(value, N_log_N, TWON_log_N), names_to = "line_type", values_to = "value") |>
    pivot_longer(c(value, N_log_N, TWON_log_N), names_to = "line_type", values_to = "value") |>
    mutate(line_type = factor(line_type, levels = labels_rearranged))

# tikzDevice::tikz(file = "./df_bis_ucmp_logs.tex", width = 7, height = 3)

color_palette <- c("#000000", "#F8766D", "#000000")
# size_palette <- c(1.5, 1, 0.75)
alpha_palette <- c(1, 1, 0.5)

df_bis.ucmp_log |>
    ggplot(aes(N, value, group = line_type)) +
    geom_line(aes(color = line_type, linetype = line_type, alpha = line_type)) +
    # geom_line(aes(N, HALFN_log_N, color = "red")) +
    scale_linetype_manual(name = "$f(n)$",
                          labels = latex_labels,
                          values = c("solid", "solid", "solid", "solid")) +
    scale_color_manual(name = "$f(n)$",
                       labels = latex_labels,
                       values = color_palette) +
    scale_alpha_manual(name = "$f(n)$",
                      labels = latex_labels,
                      values = alpha_palette) +
    labs (
        title = "Moyenne des comparaisons",
        subtitle = "$\\texttt{nombreComparaisons(n)} \\in \\Theta(n \\log n)$",
        x = "$n$",
        y = "",
        col = ""
    )

# dev.off()



# This is my attempt to merge the two tables

# df_both <- df_bis

# df_both %<>%
#   rename(mean_cmp.bis = avg_cmp,
#          mean_ech.bis = avg_ech) %>%
#   mutate(mean_cmp.a = df_a$avg_cmp,
#          mean_ech.a = df_a$avg_ech) %>%
#   gather(complexity_class,

#          complexity,
#          mean_cmp.bis,
#          mean_ech.bis,
#          mean_cmp.a,
#          mean_ech.a,
#          ONE_TENTH_log_N,
#          HALFN_log_N,
#          TWON_log_N)

latex_labels <- c("$2n \\log n$", "$n \\log n$", "$\\mu_{ech}$", "$0.5n \\log n$")
labels_rearranged <- c("TWON_log_N", "N_log_N", "value", "HALFN_log_N")

df_bis.uech_log <- stack_logs(df_bis.uech) |>
    # pivot_longer(c(value, N_log_N, TWON_log_N), names_to = "line_type", values_to = "value") |>
    pivot_longer(c(value, N_log_N, TWON_log_N, HALFN_log_N), names_to = "line_type", values_to = "value") |>
    mutate(line_type = factor(line_type, levels = labels_rearranged))


# df %<>% pivot_longer(c(value, N_log_N, TWON_log_N, ONE_TENTH_log_N), names_to = "line_type", values_to = "value")

# tikzDevice::tikz(file = "./df_bis_ucmp_logs.tex", width = 7, height = 3)

# tikzDevice::tikz(file = "./df_bis_uech.tex", width = 7, height = 3)

# color_palette <- c("#000000", "#000000", "#F8766D", "#000000")
color_palette <- c("#000000", "#000000", "#00BFC4", "#000000")
# size_palette <- c(1.5, 1, 0.75)
alpha_palette <- c(1, 0.5, 1, 0.25)

df_bis.uech_log |>
    ggplot(aes(N, value, group = line_type)) +
    geom_line(aes(color = line_type, linetype = line_type, alpha = line_type)) +
    # geom_line(aes(N, HALFN_log_N, color = "red")) +
    scale_linetype_manual(name = "$f(n)$",
                          labels = latex_labels,
                          values = c("solid", "solid", "solid", "solid")) +
    scale_color_manual(name = "$f(n)$",
                       labels = latex_labels,
                       values = color_palette) +
    scale_alpha_manual(name = "$f(n)$",
                      labels = latex_labels,
                      values = alpha_palette) +
    labs (
        title = "Moyenne des échanges",
        subtitle = "$\\texttt{nombreEchanges(n)} \\in \\Theta(n \\log n)$",
        x = "$n$",
        y = "",
        col = ""
    )

# dev.off()

# ! Compare all three values!
# tikzDevice::tikz(file = "./df_bis_all.tex", width = 7, height = 3)

# latex_labels <- c("$2n \\log n$", "$n \\log n$", "$\\mu_{ech}$")
# latex_labels <- c("$2n \\log n$", "$\\mu_{cmp} + \\mu_{ech}$", "$\\mu_{cmp}$", "$n \\log n$", "$\\mu_{ech}$", "$0.5n \\log n$")
latex_labels <- c("$2n \\log n$", "$\\mu_{cmp} + \\mu_{ech}$", "$\\mu_{cmp}$", "$\\mu_{ech}$", "$0.5n \\log n$")
# labels_rearranged <- c("twonlogn", "sum", "cmp", "nlogn", "ech", "halfn")
labels_rearranged <- c("twonlogn", "sum", "cmp", "ech", "halfn")
# alpha_palette <- c(1, 1, 1, .5, 1)
# alpha_palette <- c(1, 1, 1, .5, 1, 0.25)
alpha_palette <- c(1, 1, 1, 1, 0.25)
# color_palette <- c("#000000", "#C77CFF", "#F8766D", "#000000", "#00BFC4")
# color_palette <- c("#000000", "#C77CFF", "#F8766D", "#000000", "#00BFC4", "#000000")
color_palette <- c("#000000", "#C77CFF", "#F8766D", "#00BFC4", "#000000")
line_palette  <- rep("solid", length(latex_labels))

df_bis.all <- df_bis |>
    mutate(cmp = avg_cmp, ech = avg_ech, sum = avg_cmp + avg_ech, nlogn = N * log2(N), twonlogn = 2 * N * log2(N), halfn = 0.5 * N * log2(N)) |>
    # select(N, cmp, ech, sum, nlogn, twonlogn, halfn) |>
    select(N, cmp, ech, sum, twonlogn, halfn) |>
    # pivot_longer(c(cmp, ech, sum, nlogn, twonlogn, halfn), names_to = "line_type", values_to = "stat") |>
    pivot_longer(c(cmp, ech, sum, twonlogn, halfn), names_to = "line_type", values_to = "stat") |>
    mutate(line_type = factor(line_type, levels = labels_rearranged))

# Now plot it
df_bis.all |>
    ggplot(aes(N, stat, group = line_type)) +
    geom_line(aes(color = line_type, linetype = line_type, alpha = line_type)) +
    # geom_line(aes(N, HALFN_log_N, color = "red")) +
    scale_linetype_manual(name = "$f(n)$",
                          labels = latex_labels,
                          values = line_palette) +
    scale_color_manual(name = "$f(n)$",
                       labels = latex_labels,
                       values = color_palette) +
    scale_alpha_manual(name = "$f(n)$",
                      labels = latex_labels,
                      values = alpha_palette) +
    labs (
        title = "Moyenne des comparaisons et échanges",
        subtitle = "$\\texttt{nombresCmpPlusEch}(n) \\in \\Theta(n \\log n)$",
        x = "$n$",
        y = "",
        col = ""
    )

# dev.off()

# # df_bis.all_log <- stack_logs(df_bis.uech) |>
#     # pivot_longer(c(value, N_log_N, TWON_log_N), names_to = "line_type", values_to = "value") |>
#     pivot_longer(c(value, N_log_N, TWON_log_N, HALFN_log_N), names_to = "line_type", values_to = "value") |>
#     mutate(line_type = factor(line_type, levels = labels_rearranged))


# df %<>% pivot_longer(c(value, N_log_N, TWON_log_N, ONE_TENTH_log_N), names_to = "line_type", values_to = "value")

# tikzDevice::tikz(file = "./df_bis_ucmp_logs.tex", width = 7, height = 3)

# tikzDevice::tikz(file = "./df_bis_uech.tex", width = 7, height = 3)

# color_palette <- c("#000000", "#000000", "#F8766D", "#000000")
# size_palette <- c(1.5, 1, 0.75)


tikzDevice::tikz(file = "./df_p5_all.tex", width = 7, height = 3)
# Comparison of partitionBis and part A
# p5 for partie 5
df_p5 <- df_all.mutant |> filter(part == "a" | part == "bis") |>
    pivot_wider(names_from = stat, values_from = value)

df_p5 <- stack_logs(df_p5)

# latex_labels <- c("$2n \\log n$", "$\\mu_{cmp} + \\mu_{ech}$", "$\\mu_{cmp}$", "$\\mu_{ech}$", "$n \\log n$", "$0.1n \\log n$")
latex_labels <- c("$2n \\log n$", "$\\mu_{cmp} + \\mu_{ech}$", "$\\mu_{cmp}$", "$\\mu_{ech}$", "$0.1n \\log n$")
# labels_rearranged <- c("twon", "usum", "ucmp", "nlogn", "uech", "tenthn")
labels_rearranged <- c("twon", "usum", "ucmp", "uech", "tenthn")
# alpha_palette <- c(1, 1, 1, 0.5, 1, 0.125)
alpha_palette <- c(1, 1, 1, 0.5, 0.125)
# color_palette <- c("#000000", "#C77CFF", "#F8766D", "#000000", "#00BFC4", "#000000")
color_palette <- c("#000000", "#C77CFF", "#F8766D", "#00BFC4", "#000000")
# line_palette  <- rep("solid", length(latex_labels))

df_p5.long <- df_p5 |>
    mutate(n = N, twon = TWON_log_N, tenthn = ONE_TENTH_log_N) |>
    select(N, part, ucmp, uech, usum, twon, tenthn) |>
    pivot_longer(c(ucmp, uech, usum, twon, tenthn), names_to = "stat", values_to = "value") |>
    mutate(stat = factor(stat, levels = labels_rearranged))

# Now plot it
df_p5.long |>
    ggplot(aes(N, value, color = stat)) +
    geom_line(aes(color = stat, alpha = stat)) +
    scale_color_manual(name = "$f(n)$",
                       labels = latex_labels,
                       values = color_palette) +
    scale_alpha_manual(name = "$f(n)$",
                      labels = latex_labels,
                      values = alpha_palette) +
    labs (
        title = "Moyenne des comparaisons et échanges",
        subtitle = "$\\texttt{nombresCmpPlusEch}(n) \\in \\Theta(n \\log n)$",
        x = "$n$",
        y = "",
        col = ""
    ) +
    facet_wrap(~part)

dev.off()



# df_both |>
#   ggplot(aes(x = N_log_N, y = complexity)) +
#   geom_line(aes(color = complexity_class)) +
#   #scale_color_manual(labels = c("A", "B", "C", "D", "E", "F", "G"), values = c("A", "B", "C", "D", "E", "F", "G"))
#   #scale_fill_discrete(name = "$f(n)$", labels = c ("A", "B", "C", "D", "E", "F", "G")) +
#   labs(col="$f(n)$") +
#   scale_color_hue(labels = c("$0.5 n\\log n$", "B", "C", "D", "E", "F", "G")) +
#   ggtitle("Complexity Analysis")

#============= Linear Regression Analysis ================#
# Get the mx and b value of a linear regression given
# y and x data
lin_reg <- function(formula, data) {
  reg <- lm(formula, data = data)
  b <- reg$coefficients[1]
  m <- reg$coefficients[2]
  return(\(x) m * x + b)
}

rms <- function (y, y_hat) {
  sqrt(mse(y, y_hat))
}

# Calculate error MSE
mse <- function(y, y_hat) {
  err <- y - y_hat
  err <- err * err
  mean(err)
}

# tikzDevice::tikz(file = "./plot.tex", width = 10, height = 5)


# Create the linear regression plot

df <- df_bis.ucmp
f <- lin_reg(value~N_log_N, data = df)

model <- tibble(x = df$N_log_N,
                y = f(x))
# str(model)

df %<>%
  mutate(val_vs_nlogn = f(N_log_N)) %<>%
  mutate(link = as.factor(1:length(N_log_N)))


# tikzDevice::tikz(file = "./df_bis_ucmp_reg.tex", width = 7, height = 3)

df |>
  ggplot(aes(N_log_N, value)) +
  geom_point(size=0.25) +
  geom_line(aes(N_log_N, val_vs_nlogn), size = 0.25) +
  geom_line(aes(N_log_N, value), color = "red", alpha = 0.3) +
  geom_segment(aes(N_log_N, val_vs_nlogn, xend = N_log_N, yend = value), color="red") +
  labs(
      title = "Moyenne des comparaisons",
      subtitle = "$\\texttt{nombreComparaisons}(n) \\approx 1.16(n\\log n) - 549.56$",
      x = "$n \\log n$",
      y = ""
    #   y = "$f(x) = 1.16x - 549.56$"
    )
dev.off()

#   theme(plot.title = element_text(hjust = 0.5))  # Center the title
#   geom_label(x = 300, y = 30, label="Hello")
df.mse <- mse(df$value, df$val_vs_nlogn)
df.rms <- rms(df$value, df$val_vs_nlogn)


# dev.off()

## Here we can plot against the number of exchanges
