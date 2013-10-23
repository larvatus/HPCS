

m1 = matrix( rep(rnorm(125,0,1), 30), ncol = 30, byrow = F )

m2 = matrix( rep(rnorm(125,0,2), 30), ncol = 30, byrow = F )

write.table( m1, file = 'dataSet.3.dat', row.names = F, col.names = F )

write.table( m2, file = 'dataSet.4.dat', row.names = F, col.names = F )

