#!/usr/bin/Rscript

library(fda)

pathToData = '../../../synthetic/outliersSine/'
#pathToData = '../../../../leads/'

I = read.table( paste( pathToData, "I.txt", sep = ''), header = F )
II = read.table( paste( pathToData, "II.txt", sep = ''), header = F )

#I = read.table( paste( pathToData, "I.txt", sep = ''), header = F )
#II = read.table( paste( pathToData, "II.txt", sep = ''), header = F )
#V1 = read.table( paste( pathToData, "V1.txt", sep = ''), header = F )
#V2 = read.table( paste( pathToData, "V2.txt", sep = ''), header = F )
#V3 = read.table( paste( pathToData, "V3.txt", sep = ''), header = F )
#V4 = read.table( paste( pathToData, "V4.txt", sep = ''), header = F )
#V5 = read.table( paste( pathToData, "V5.txt", sep = ''), header = F )
#V6 = read.table( paste( pathToData, "V6.txt", sep = ''), header = F )

depth = read.table( 'mbd.dat')

Dpt = depth[,-1]

pdf('fbplot.pdf',width = 10)
par( mfrow = c(2,1) )
#par( mfrow = c(2,4) )
matplot( t(I), type = 'l' )
fbplot( t(I), depth = Dpt, plot = T, ylim = range(I), xlab = 'Sampling time', ylab = '')
#fbplot( t(II), depth = Dpt, plot = T, ylim = range(II), xlab = 'Sampling time', ylab = 'II' )
#fbplot( t(V1), depth = Dpt, plot = T, ylim = range(V1), xlab = 'Sampling time', ylab = 'V1' )
#fbplot( t(V2), depth = Dpt, plot = T, ylim = range(V2), xlab = 'Sampling time', ylab = 'V2' )
#fbplot( t(V3), depth = Dpt, plot = T, ylim = range(V3), xlab = 'Sampling time', ylab = 'V3')
#fbplot( t(V4), depth = Dpt, plot = T, ylim = range(V4), xlab = 'Sampling time', ylab = 'V4' )
#fbplot( t(V5), depth = Dpt, plot = T, ylim = range(V5), xlab = 'Sampling time', ylab = 'V5' )
#fbplot( t(V6), depth = Dpt, plot = T, ylim = range(V6), xlab = 'Sampling time', ylab = 'V6' )
dev.off()
