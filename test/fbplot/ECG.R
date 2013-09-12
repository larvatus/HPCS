#!/usr/bin/Rscript

library(fda)


pathToData = '../../../leads/'

I = read.table( paste( pathToData, "I.txt", sep = ''), header = F )
II = read.table( paste( pathToData, "II.txt", sep = ''), header = F )
V1 = read.table( paste( pathToData, "V1.txt", sep = ''), header = F )
V2 = read.table( paste( pathToData, "V2.txt", sep = ''), header = F )
V3 = read.table( paste( pathToData, "V3.txt", sep = ''), header = F )
V4 = read.table( paste( pathToData, "V4.txt", sep = ''), header = F )
V5 = read.table( paste( pathToData, "V5.txt", sep = ''), header = F )
V6 = read.table( paste( pathToData, "V6.txt", sep = ''), header = F )

time = seq(1,700);

pdf('preproc_ecg.pdf', width = 12, height = 7 )
par( mfrow = c(2,4) )
matplot( time, t(I),  type = 'l', xlab = 'sampl. time [ms]', ylab = 'I' )
matplot( time, t(II), type = 'l', xlab ='sampl. time [ms]', ylab = 'II' )
matplot( time, t(V1), type = 'l',  xlab = 'sampl. time [ms]', ylab = 'V1' )
matplot( time, t(V2), type = 'l', xlab = 'sampl. time [ms]', ylab = 'V2' )
matplot( time, t(V3), type = 'l', xlab = 'sampl. time [ms]', ylab = 'V3' )
matplot( time, t(V4), type = 'l', xlab = 'sampl. time [ms]', ylab = 'V4' )
matplot( time, t(V5), type = 'l',  xlab = 'sampl. time [ms]', ylab = 'V5' )
matplot( time, t(V6), type = 'l', xlab = 'sampl. time [ms]', ylab = 'V6' )
dev.off()


IDs = seq(1,149);
saneIDs = IDs <= 101;
illIDs = IDs >= 102;

fact = as.factor( as.numeric(saneIDs) + as.numeric(illIDs) )
factnum = as.numeric(fact);
factnum[saneIDs] = factnum[saneIDs]+3;
factnum[illIDs] = factnum[illIDs]+1;

pdf('sani_malati_ECG.pdf', width = 12, height = 7 )
par( mfrow = c(2,4) )
matplot( time, t(I),  type = 'l', col = factnum, xlab = 'sampl. time [ms]', ylab = 'I' )
matplot( time, t(II),  type = 'l', col = factnum, xlab ='sampl. time [ms]', ylab = 'II' )
matplot( time, t(V1),  type = 'l', col = factnum, xlab = 'sampl. time [ms]', ylab = 'V1' )
matplot( time, t(V2), type = 'l', col = factnum, xlab = 'sampl. time [ms]', ylab = 'V2' )
matplot( time, t(V3), type = 'l', col = factnum, xlab = 'sampl. time [ms]', ylab = 'V3' )
matplot( time, t(V4), type = 'l', col = factnum, xlab = 'sampl. time [ms]', ylab = 'V4' )
matplot( time, t(V5), type = 'l', col = factnum, xlab = 'sampl. time [ms]', ylab = 'V5' )
matplot( time, t(V6), type = 'l', col = factnum, xlab = 'sampl. time [ms]', ylab = 'V6' )
dev.off()

