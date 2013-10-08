####################################
##
##  REGRESSION AND CLASSIFICATION
##
####################################

rm( list = ls() );

####################################
## SETUP
####################################

J = 2;

nbPz = 99;

distance = 'uniform';

# Every patient with ID (strictly) less than saneThresholdID is Sane, the others are not.
saneThresholdID = 52;
# Cutting probability for the conversion of the logistic model to a classifier.
probThreshold = 0.5;

####################################
## READING FILES
###################################

MBDPri = read.csv( './mbd.1.dat', sep = '', header = FALSE );
MBDDer = read.csv( './mbd.der.1.dat', sep = '', header = FALSE );

stopifnot( dim(MBDPri)[1] == nbPz && dim(MBDDer)[1] == nbPz )
  
isSick = ifelse( MBDPri[,1] < saneThresholdID, 0, 1) 

MBDPri = MBDPri[,-1];
MBDDer = MBDDer[,-1];

RksPri = rank( MBDPri );
RksDer = rank( MBDDer );

#############################################
## REGRESSION WITH MBD
#############################################

fitMbd = glm( isSick ~ MBDPri + MBDDer, family = binomial(logit) )

summary(fitMbd)

fittedMbd = fitMbd$fitted.values;

labelsMbd = ifelse( fittedMbd > probThreshold, 1., 0 );

x11()
#pdf( paste('res/img/logistic_J', J, '_MBD_', distance, '.pdf', sep = '') );
par( mfrow = c(1,2) );
plot( fittedMbd, main = paste( 'Full Log MBD - ', distance, ' wghts', sep = ''), xlab = 'patients' )
points( isSick, pch = 21, col = 'black', bg = 'cyan' )

miscIDsMbd = which( isSick != labelsMbd )
matchIDsMbd = which( isSick == labelsMbd );

miscSaneAsSickMbd = length( which( labelsMbd[ 1 : (saneThresholdID-1) ] != isSick[ 1 : (saneThresholdID-1)] ) );
miscSickAsSaneMbd = length( which( labelsMbd[ saneThresholdID : nbPz ] != isSick[ saneThresholdID : nbPz ] ) ); 

points( x = miscIDsMbd, y = fittedMbd[ miscIDsMbd ], type = 'p', pch = 21, col = 'black', bg = 'red' )
points( x = matchIDsMbd, y = fittedMbd[ matchIDsMbd ], type = 'p', pch = 21, col = 'black', bg = 'green' )
abline(0.5, 0, col = 'blue', lty = 'dashed', lwd = 2)

######## REDUCED: Only traces, not derivatives

fitMbdRed = glm( isSick ~  MBDPri, family = binomial(logit) )

summary(fitMbdRed)

fittedMbdRed = fitMbdRed$fitted.values;

labelsMbdRed = ifelse( fittedMbdRed > probThreshold, 1., 0 );

pdf('logistic.pdf')
#plot( fittedMbdRed, main = paste( 'Red. Log MBD - ', distance,' wghts', sep = ''), xlab = 'patients' )
plot( fittedMbdRed, main = 'Log MBD - pesi uniformi', xlab = 'Soggetti', ylab = 'Valori predetti' )
points( isSick, pch = 21, col = 'black', bg = 'cyan', cex =2 )

miscIDsMbdRed = which( isSick != labelsMbdRed )
matchIDsMbdRed = which( isSick == labelsMbdRed );

miscSaneAsSickMbdRed = length( which( labelsMbdRed[ 1 : (saneThresholdID-1) ] != isSick[ 1 : (saneThresholdID-1)] ) );
miscSickAsSaneMbdRed = length( which( labelsMbdRed[ saneThresholdID : nbPz ] != isSick[ saneThresholdID : nbPz ] ) ); 

points( x = miscIDsMbdRed, y = fittedMbdRed[ miscIDsMbdRed ], type = 'p', cex =2, pch = 21, col = 'black', bg = 'red' )
points( x = matchIDsMbdRed, y = fittedMbdRed[ matchIDsMbdRed ], type = 'p', cex =2, pch = 21, col = 'black', bg = 'green' )
abline(0.5, 0, col = 'blue', lty = 'dashed', lwd = 2)
dev.off();

##################################################
## REGRESSION WITH RANKS
#################################################

fitRks = glm( isSick ~ RksPri + RksDer, family = binomial(logit) )

summary(fitRks)

fittedRks = fitRks$fitted.values;

labelsRks = ifelse( fittedRks > probThreshold, 1., 0 );

x11()
#pdf( paste('res/img/logistic_J', J, '_ranks_', distance,'.pdf', sep = '') );
par( mfrow = c(1,2) );
plot( fittedRks, main = paste( 'Full Log RKS - ', distance, ' wghts', sep = ''), xlab = 'patients' )
points( isSick, pch = 21, col = 'black', bg = 'cyan' )

miscIDsRks = which( isSick != labelsRks )
matchIDsRks = which( isSick == labelsRks );

miscSaneAsSickRks = length( which( labelsRks[ 1 : (saneThresholdID-1) ] != isSick[ 1 : (saneThresholdID-1)] ) );
miscSickAsSaneRks = length( which( labelsRks[ saneThresholdID : nbPz ] != isSick[ saneThresholdID : nbPz ] ) ); 

points( x = miscIDsRks, y = fittedRks[ miscIDsRks ], type = 'p', pch = 21, col = 'black', bg = 'red' )
points( x = matchIDsRks, y = fittedRks[ matchIDsRks ], type = 'p', pch = 21, col = 'black', bg = 'green' )
abline(0.5, 0, col = 'blue', lty = 'dashed', lwd = 2)

############### REDUCED: only traces, not derivatives

fitRksRed = glm( isSick ~ RksPri, family = binomial(logit) )

summary(fitRksRed)

fittedRksRed = fitRksRed$fitted.values;

labelsRksRed = ifelse( fittedRksRed > probThreshold, 1., 0 );

plot( fittedRksRed, main = paste( 'Red. Log RKS - ', distance, ' wghts', sep = ''), xlab = 'patients' )
points( isSick, pch = 21, col = 'black', bg = 'cyan' )

miscIDsRksRed = which( isSick != labelsRksRed )
matchIDsRksRed = which( isSick == labelsRksRed );

miscSaneAsSickRksRed = length( which( labelsRksRed[ 1 : (saneThresholdID-1) ] != isSick[ 1 : (saneThresholdID-1)] ) );
miscSickAsSaneRksRed = length( which( labelsRksRed[ saneThresholdID : nbPz ] != isSick[ saneThresholdID : nbPz ] ) ); 

points( x = miscIDsRksRed, y = fittedRksRed[ miscIDsRksRed ], type = 'p', pch = 21, col = 'black', bg = 'red' )
points( x = matchIDsRksRed, y = fittedRksRed[ matchIDsRksRed ], type = 'p', pch = 21, col = 'black', bg = 'green' )
abline(0.5, 0, col = 'blue', lty = 'dashed', lwd = 2)

#dev.off();
###################################

paste( paste( '******* Misclassified full model MBD ', distance, ' : ', length(miscIDsMbd), ', ', miscSaneAsSickMbd, ' Sane and ', miscSickAsSaneMbd,' Sick', sep = '') );
paste( paste( '******* Misclassified red model MBD ', distance, ' : ', length(miscIDsMbdRed), ', ', miscSaneAsSickMbdRed, ' Sane and ', miscSickAsSaneMbdRed,' Sick', sep = '') );
paste( paste( '******* Misclassified full model RKS ', distance, ' : ', length(miscIDsRks), ', ', miscSaneAsSickRks, ' Sane and ', miscSickAsSaneRks,' Sick', sep = '') );
paste( paste( '******* Misclassified red model RKS ', distance, ' : ', length(miscIDsRksRed), ', ', miscSaneAsSickRksRed, ' Sane and ', miscSickAsSaneRksRed,' Sick', sep = '') );
