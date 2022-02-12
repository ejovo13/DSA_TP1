library(tidyverse)
library(magrittr)
library(ggthemes)
library(scales)
library(viridis)
library(wesanderson) # For more color palettes
#library(tikzDevice)

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

prefix = "/home/ejovo/MAIN/S6/DSA/DSA_TP1/"

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



# dev.off()

## Here we can plot against the number of exchanges