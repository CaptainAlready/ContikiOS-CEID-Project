#Script used to visualize energest values after running rpl & simplenode on Cooja using 
#z1 mote

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

#prot used for changing plots, just change the index from 0 to 1 and viseversa
prot = ['energest_logrpl.csv','energest_logsimplenode.csv']

#do everything in one line for the memes
#read csv , select relevant fields, sum them by id and divide with RTimer constant
df = pd.read_csv(prot[1], sep=';').groupby('id')["CPU","LPM","Radio_TX","Radio_RX"].sum()/32768

#Calculate average Current(Iavg) for each mote

##################################
###FUNCTION# Current Consumption##
##   CPU   #          10mA      ##
##   LPM   #         0.5mA      ##
##   Tx    #        17.4mA      ##
##   Rx    #        18.8mA      ##
##################################

#create a series contaning the consumption values
consumption = pd.Series([10,0.5,17.4,18.8],index=["CPU","LPM","Radio_TX","Radio_RX"])

#calculate Iavg and store it in dataframe
#division with 1440 because 60sec * 24(24 min simulation) = 1440 sec
#Iavg = CPU*t(CPU) + LPM*t(LPM) + Tx*t(Tx) + Rx*t(Rx) 
df = df*consumption/1440
Iavg = df.sum(axis=1, numeric_only= True)

#calculate Uptime 
#Uptime = 40mAh/Iavg 
Uptime = 40/Iavg

#plot Uptime
Uptime.plot.bar() 
plt.ylabel('Uptime(Hours)')
plt.xlabel('Mote ID')
title = 'Uptime of each mote using a 40mAh battery, given ' + prot[1] 
plt.title(title)
plt.show()