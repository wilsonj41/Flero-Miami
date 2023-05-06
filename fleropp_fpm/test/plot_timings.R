library(ggplot2)
library(scales)
library(gghighlight)

data_summary <- function(data, varname, groupnames){
  require(plyr)
  summary_func <- function(x, col){
    c(mean = mean(x[[col]], na.rm=TRUE),
      sd = sd(x[[col]], na.rm=TRUE))
  }
  data_sum<-ddply(data, groupnames, .fun=summary_func,
                  varname)
  data_sum <- rename(data_sum, c("mean" = varname))
  return(data_sum)
}

cmdline <- read.csv("./cmdline_timing.dat", header = T)
cmdline$program <- "cmdline"
fleropp <- read.csv("./fleropp_timing.dat", header = T)
fleropp$walltime <- round(fleropp$time_total, digits = 3)
fleropp <- fleropp[c("n", "walltime")]
fleropp$program <- "fleropp"
laravel <- read.csv("./laravel_timing.dat", header = T)
laravel$walltime <- round(laravel$time_total, digits = 3)
laravel <- laravel[c("n", "walltime")]
laravel$program <- "laravel"

data <- rbind(fleropp, cmdline, laravel)

data_summ <- data_summary(data, varname = "walltime", groupnames = c("program", "n"))

ggplot(data_summ, aes(x = n, y = walltime, color = program)) +
  geom_line() +
  geom_errorbar(aes(ymin = walltime - sd, ymax = walltime + sd), width = 0.2,
                    position = position_dodge(0.05)) +
  scale_x_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))
  ) +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))
  ) +
  labs(title = "n-Primes Benchmark", x = "n", y = "Response Time (seconds)",
       color = "Framework") +
  scale_color_manual(labels = c("Flero++", "Stock C++", "PHP Laravel"), 
                     values = c("#66A182", "#5E97D0", "#FB503B")) +
  annotation_logticks() +
  theme_bw() +
  theme(panel.background = element_rect(color = "black", size = 1),
        legend.title = element_text(face = "bold"),
        plot.title = element_text(face = "bold"))


ggsave("./bench.png", dpi = 320, width = 10, height = 6, units = "in")

hello <- read.csv("./hello_timing.dat", header = T)
hello_summ <- data_summary(hello, varname = "time_total", groupnames = c("program"))

ggplot(hello_summ, aes(x = reorder(program, -time_total), y = time_total * 1000, fill = program)) +
  geom_col() +
  labs(title = "Hello World Benchmark", x = "Framework", y = "Mean Response Time (milliseconds)",
       color = "Framework") +
  scale_fill_manual(labels = c("PHP Laravel", "Flero++"), 
                     values = c("#66A182", "#FB503B")) +
  scale_x_discrete(labels = c("PHP Laravel", "Flero++")) +
  theme_bw() +
  theme(panel.background = element_rect(color = "black", size = 1),
        legend.title = element_text(face = "bold"),
        plot.title = element_text(face = "bold"),
        legend.position = "none",
        axis.text.x = element_text(size = 12, face = "bold", color = "black"),
        axis.title.x = element_blank())

ggsave("./hello_bench.png", dpi = 320, width = 10, height = 6, units = "in")
  
