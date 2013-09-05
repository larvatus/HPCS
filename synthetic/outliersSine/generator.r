###########################################
## Bivariate functional family generator ##
###########################################

rm ( list = ls() );

### LOADING MULTIVARIATE NORMAL DISTRIBUTION LIBRARY

library(mvtnorm)

### PARAMETERS ###

N = 60;
PerturbedThresholdID = 30;
NPerturbed = 10;

S = 2;

T0 = 0.;
T1 = 2.;

meanLowerNoise = c(0., 0.);
rhoLowerNoise = 0.9;
covLowerNoise = matrix( c(1., rhoLowerNoise, rhoLowerNoise, 1.), ncol = 2, byrow = TRUE );

lowerAmplitude = 1;
lowerAmplitudeMaxWidth = 1.5;
widthPhaseNoise = 0.4;

widthMaxShapeNoise = 5;
widthShapeMean = 2;

nTimeSamples = 500;
sampleInsertionThreshold = 350;

freq = 1.; 

T = c(T0,T1);

times = seq( from = T[1], to = T[2], len = nTimeSamples )

### GENERATING RANDOM PERTURBATIONS AND NOISE

lowerAmpliNoise = lowerAmplitude + ( lowerAmplitudeMaxWidth - lowerAmplitude ) * rmvnorm( N, meanLowerNoise, covLowerNoise);
lowerPhaseNoise = widthPhaseNoise * rmvnorm( N, meanLowerNoise, covLowerNoise);


x11()
D = 2.5;
plot( LowerNoise, xlim = c(-D,D), ylim = c(-D,D) )

### GENERATING DATA

widthPerturbation =  runif( NPerturbed, widthShapeMean, widthMaxShapeNoise ) * sample( c(-1,+1), NPerturbed, replace = TRUE )
timeNoiseInsertion = runif( NPerturbed, 0, sampleInsertionThreshold)

dataI = matrix( NaN, nrow = N, ncol = nTimeSamples, byrow = TRUE )
dataII = matrix( NaN, nrow = N, ncol = nTimeSamples, byrow = TRUE );

for( iN in 1 : (N) )
{
  dataI[ iN, ] =  lowerAmpliNoise[ iN, 1 ] * sin( 2 * pi * times + lowerPhaseNoise[ iN, 1 ] );

  dataII[ iN, ] =  lowerAmpliNoise[ iN, 2 ] * sin( 2 * pi * times + lowerPhaseNoise[ iN, 2 ] );

}

for ( iN in 1 : NPerturbed )
{  
  print( N-iN+1)
  for ( iPt in floor(timeNoiseInsertion[ iN ]) : nTimeSamples  )
  {
    dataI[ N - iN + 1, iPt] =  dataI[ N - iN + 1, iPt ] + widthPerturbation[ iN ];
    
    dataII[ N - iN + 1, iPt] =  dataII[ N - iN + 1, iPt ] + widthPerturbation[ iN ];
  }  
}


### VISUALIZING DATA
x11()
par( mfrow = c(2,1) )
matplot( t(dataI), type = 'l')
matplot( t(dataII), type = 'l' );


### SAVING IMAGES
postscript('I_outliers.ps');
matplot( t(dataI), type = 'l');
dev.off();
postscript('II_outliers.ps');
matplot( t(dataII), type = 'l' );
dev.off();

### SAVING DATA
write.table( dataI, file = "I.txt", sep = " ", eol = "\n", row.names = FALSE, col.names = FALSE );
write.table( dataII, file = "II.txt", sep = " ", eol = "\n", row.names = FALSE, col.names = FALSE);
