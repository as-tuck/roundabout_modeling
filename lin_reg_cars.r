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

# Convert from tics to seconds
data$car.flow <- data$car.flow * (8/3)

# Partition data based on direction of traffic
data.exit <- data[data$direction == 'exit', ]
data.entry <- data[data$direction == 'entry', ]

# Create linear models for analysis
  # While not used directly in the graphs, these models should be identical to the ones therein
model.entry <- lm(formula= car.flow ~ pedestrian.count, data= data.entry)
model.exit <- lm(formula= car.flow ~ pedestrian.count, data= data.exit)

# Plot and save the models vs their actual data points
ggplot(data.entry, aes(x= pedestrian.count, y= car.flow)) +
  labs(title= "Number of Pedestrians vs Flow Rate of Cars Entering a Roundabout",
       x= "Number of Pedestrians (count)",
       y= "Flow Rate of Cars (cars/s)") +
  ylim(0,12) +
  geom_point() +
  geom_smooth(method= lm, se= FALSE)
ggsave("entry_plot.png")

ggplot(data.exit, aes(x= pedestrian.count, y= car.flow)) +
  labs(title= "Number of Pedestrians vs Flow Rate of Cars Exiting a Roundabout",
       x= "Number of Pedestrians (count)",
       y= "Flow Rate of Cars (cars/s)") +
  ylim(0,12) +
  geom_point() +
  geom_smooth(method= lm, se= FALSE)
ggsave("exit_plot.png")

data.entry$predicted.flow <- predict(object= model.entry, newdata= data.entry)
abs.err.entry <- abs(data.entry$car.flow - data.entry$predicted.flow)
mean.abs.err.entry <- mean(abs.err.entry)
summary(model.entry)
print(mean.abs.err.entry)

data.exit$predicted.flow <- predict(object= model.exit, newdata= data.exit)
abs.err.exit <- abs(data.exit$car.flow - data.exit$predicted.flow)
mean.abs.err.exit <- mean(abs.err.exit)
summary(model.exit)
print(mean.abs.err.exit)
