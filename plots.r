library(tidyverse)
library(magrittr)
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

fbis   <- "/home/ejovo/MAIN/S6/DSA/p_Bis.csv"
fa     <- "/home/ejovo/MAIN/S6/DSA/p_Hoare.csv"
f      <- "/home/ejovo/MAIN/S6/DSA/p_Lomuto.csv"
fs     <- "/home/ejovo/MAIN/S6/DSA/p_Sedgewick.csv"

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

df_all

df_all.ucmp <- df_all |>
                pivot_longer(c(ucmp.a, ucmp.bis, ucmp.lum, ucmp.sed), names_to = "partition_type", values_to = "ucmp")

df_all.uech <- df_all |>
                pivot_longer(c(uech.a, uech.bis, uech.lum, uech.sed), names_to = "partition_type", values_to = "uech")

df_all.usum <- df_all |>
                pivot_longer(c(usum.a, usum.bis, usum.lum, usum.sed), names_to = "partition_type", values_to = "usum")

# Plot all of the average comparisons
df_all.ucmp |>
    ggplot(aes(N, ucmp, color = partition_type)) +
    geom_line() +
    labs(title="Average comparisons", subtitle="How Partition scheme affects quicksort", col="Partition Scheme") +
    ylab("$\\mu$ comparaisons") +
    xlab("$n$") +
    scale_color_hue(labels = c("Parti A", "Bis", "Lomuto", "Sedgewick"))

# Plot the average exchanges
df_all.uech |>
    ggplot(aes(N, uech, color = partition_type)) +
    geom_line() +
    labs(title="Average exchanges", subtitle="How Partition scheme affects quicksort", col="Partition Scheme") +
    ylab("$\\mu$ exchanges") +
    xlab("$n$") +
    scale_color_hue(labels = c("Parti A", "Bis", "Lomuto", "Sedgewick"))

df_all.usum |>
    ggplot(aes(N, usum, color = partition_type)) +
    geom_line() +
    labs(title="Average sum of exchanges and comparisons", subtitle="How Partition scheme affects quicksort", col="Partition Scheme") +
    ylab("$\\mu_k + \\mu_x$ ") +
    xlab("$n$") +
    scale_color_hue(labels = c("Parti A", "Bis", "Lomuto", "Sedgewick"))

# I want a plot that distinguishes between exchanges and comparisons!
# df_all.cmp_ech <- df_all |>
#     pivot_longer(c(ucmp.a, ucmp.bis, ucmp.lum, ucmp.sed), names_to = "part.cmp", values_to = "ucmp") |>
#     pivot_longer(c(uech.a, uech.bis, uech.lum, uech.sed), names_to = "part.ech", values_to = "uech")

# df_all.cmp_ech |>
#     ggplot(aes(N)) +
#     geom_line(aes(N, uech), color = "red", group = df_all.cmp_ech$part.ech) +
#     geom_line(aes(N, ucmp), color = "blue", group = df_all.cmp_ech$part.cmp)

df_all.cmp_ech_sum <- df_all |>
    pivot_longer(c(ucmp.a, ucmp.bis, ucmp.lum, ucmp.sed), names_to = "part.cmp", values_to = "ucmp") |>
    pivot_longer(c(uech.a, uech.bis, uech.lum, uech.sed), names_to = "part.ech", values_to = "uech") |>
    pivot_longer(c(usum.a, usum.bis, usum.lum, usum.sed), names_to = "part.sum", values_to = "usum")

df_all.cmp_ech_sum |>
    ggplot(aes(N)) +
    geom_line(aes(N, uech), color = "red", group = df_all.cmp_ech_sum$part.ech) +
    geom_line(aes(N, ucmp), color = "blue", group = df_all.cmp_ech_sum$part.cmp) +
    geom_line(aes(N, usum), color = "orange", group = df_all.cmp_ech_sum$part.sum)


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

# Linear regression for nlogn complexity
#f_nlogn <- lin_reg(avg_cmp~N_log_N, data = df)

#df %<>%
#  mutate(regr = f_nlogn(N_log_N))

#g1 = ggplot(df, aes(N_log_N, avg_cmp)) +
#  geom_point() +
  #geom_smooth(color = "red") +
  #geom_point(aes(disp, disp_v_mpg, color = "orange"))
#  geom_line(aes(N_log_N, regr)) +
#  geom_segment(aes(N_log_N, regr, xend = N_log_N, yend = avg_cmp), color="red") +
#  theme(plot.title = element_text(hjust = 0.5)) + # Center the title
#  geom_label(x = 300, y = 30, label="Hello") +
#  ggtitle("$f(x, \\alpha, \\beta, \\gamma)$") +
#  ylab("$k_k$") +
#  xlab("$n_k\\log n_k$")##
##
#
#reg <- lm(avg_cmp~N_log_N, data = df)
#print(reg)

#tikzDevice::tikz(file = "./example_plot.tex", width = 5, height = 3)

#g1
#dev.off()

## Here we can plot against the number of exchanges