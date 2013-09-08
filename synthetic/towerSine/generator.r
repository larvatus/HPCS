###########################################
## Bivariate functional family generator ##
###########################################

rm ( list = ls() );

### PARAMETERS ###

N = 100;

S = 2;

T0 = 0.;
T1 = 2.;

nTimeSamples = 500;

freq = 1.; 

T = c(T0,T1);

times = seq( from = T[1], to = T[2], len = nTimeSamples )

### GENERATING DATA

dataI = matrix( NaN, nrow = N, ncol = nTimeSamples, byrow = TRUE )
dataII = matrix( NaN, nrow = N, ncol = nTimeSamples, byrow = TRUE );

for( iN in 1 : N )
{
  dataI[ iN, ] =   0.2*iN + sin( 2 * pi * times );

  dataII[ iN, ] =  0.2*iN + sin( 2 * pi * times );

}

### VISUALIZING DATA
x11()
par( mfrow = c(2,1) )
matplot( t(dataI), type = 'l')
matplot( t(dataII), type = 'l' );

##### FINER
x11()
par(mfrow = c(2,1) )
matplot( t(dataI[ 1:4,]), type = 'l');
matplot( t(dataII[ 1:4, ]), type = 'l');

### SAVING IMAGES
pdf('I_tower.pdf');
matplot( t(dataI), type = 'l', xlab = F, ylab = F);
dev.off();
postscript('II_tower.ps');
matplot( t(dataII), type = 'l', xlab = F, ylab = F);
dev.off();

### SAVING DATA
write.table( dataI, file = "I.txt", sep = " ", eol = "\n", row.names = FALSE, col.names = FALSE );
write.table( dataII, file = "II.txt", sep = " ", eol = "\n", row.names = FALSE, col.names = FALSE);
