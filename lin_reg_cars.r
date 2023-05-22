#######################################
# Andrew Tucker
# linear regression model of roundabout
# traffic flow vs number of pedestrians
#######################################

# Install ggplot2 for plotting in R
install.packages("jtools")
install.packages("ggplot2"); 
install.packages("gridExtra");
install.packages("MASS");

# set working directory to this file
setwd(dirname(rstudioapi::getSourceEditorContext()$path))

# Load the ggplot2 library
library(ggplot2)
library(gridExtra)
library(jtools)
library(MASS)

data <- read.csv(".\\data\\cardata2.csv")

data.exit <- data[data$direction == 'exit', ]
data.entry <- data[data$direction == 'entry', ]

model.entry <- lm(formula= car.flow ~ pedestrian.count, data= data.entry)
model.exit <- lm(formula= car.flow ~ pedestrian.count, data= data.exit)

ggplot(data.entry, aes(x= pedestrian.count, y= car.flow)) +
  labs(title= "Number of Pedestrians vs Flow Rate of Cars Entering a Roundabout") +
  ylim(0,1) +
  geom_point() +
  geom_smooth(method= lm, se= FALSE)
ggsave("entry_plot.png")

ggplot(data.exit, aes(x= pedestrian.count, y= car.flow)) +
  labs(title= "Number of Pedestrians vs Flow Rate of Cars Exiting a Roundabout") +
  ylim(0,1) +
  geom_point() +
  geom_smooth(method= lm, se= FALSE)
ggsave("exit_plot.png")
