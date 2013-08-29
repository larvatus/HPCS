###########################################
## Bivariate functional family generator ##
###########################################

rm ( list = ls() );


### PARAMETERS ###

N = 30;
N1 = 25;
N2 = 5;

T0 = 0.; T1 = 2.;

nTimeSamples = 500;

freq = 1.; 

###############

T = c(T0,T1);

times = seq( from = T[1], to = T[2], len = nTimeSamples )

dataI = matrix( NaN, nrow = N, ncol = nTimeSamples, byrow = TRUE )
dataII = matrix( NaN, nrow = N, ncol = nTimeSamples, byrow = TRUE );

for( iN in seq(1,N1) )
{
  dataI[ iN, ] =  sin( 2 * pi * times );
  dataII[ iN, ] = sin( 2 * pi * times );
}

for ( iN in seq( N1+1,N1 +N2) )
{
  dataI[ iN, ] = 2 + sin( 2 * pi * times );
  dataII[ iN, ] = 2 + sin( 2 * pi * times );
}

### Visualizing data
x11()
matplot( t(dataI), type = 'l')
x11()
matplot( t(dataII), type = 'l' );

### Saving images
postscript('I_some_outliers.ps');
matplot( t(dataI), type = 'l')
dev.off();
postscript('II_some_outliers.ps');
matplot( t(dataII), type = 'l' );
dev.off();

### Saving data
write.table( dataI, file = "I.txt", sep = " ", eol = "\n", row.names = FALSE, col.names = FALSE );
write.table( dataII, file = "II.txt", sep = " ", eol = "\n", row.names = FALSE, col.names = FALSE);
