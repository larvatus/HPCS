# 
# setwd('/home/nicholas/University/Laurea_magistrale/Anno_II/Thesis/Anna_Fra_Nic/MBD_ECG/class/build/test/wilcoxon');
# 
# rm( list = ls() )
# 
# nTrials = 10;
# nbRefPz = 70;
# trials = seq(1,nTrials);
# 
# pvalues = rep(0,nTrials);
# 
# x11()
# #pdf('composito.pdf')
# for ( iTrial in trials )
# {
#   mbdRefSet = read.csv( paste( 'mbd.', iTrial, '.refset.dat', sep = ''), header = F, sep = "\t")
#   mbdTestSet = read.csv( paste( 'mbd.', iTrial, '.testset.dat', sep = ''), header = F, sep = "\t")
#   
#   IDRefSet = mbdRefSet[,1] + 1;
#   IDTestSet = mbdTestSet[,1] + 1;
#   realIDThreshold = 31;
#   IDRealTestSet = IDTestSet[ IDTestSet < realIDThreshold ]
#   IDSynthTestSet = IDTestSet[ IDTestSet >= realIDThreshold ]
#   mbdRefSet = mbdRefSet[,-1]
#   mbdTestSet = mbdTestSet[,-1]
#   
#   plot(mbdRefSet, col = 'blue', pch = 16, xlim = range(c(1,nbRefPz+IDTestSet)), xlab = 'Soggetti', ylab = 'MBD' )
#   factCol = c( rep(2,length(IDRealTestSet)), rep(3,length(IDSynthTestSet)) ) 
#   points( nbRefPz + IDTestSet ,mbdTestSet, col = factCol, pch = 16 )
#   
#   
#   ranksRef = rep(0,length(IDTestSet))
#   
#   for ( iTest in IDTestSet )
#   {
#     
#     for ( iRef in IDRefSet )
#     {
#       if ( mbdTestSet[ iTest ] > mbdRefSet[ iRef ] )
#       {
#         ranksRef[ iTest ] = ranksRef[ iTest ] + 1;
#       }
#     }
#   }
#   
#   ranksRef = ranksRef / length(mbdRefSet)
#   
#   ranksInduced = rank(ranksRef)
#   
#   test = wilcox.test(ranksInduced[ IDSynthTestSet ], ranksInduced[ IDRealTestSet], paired = FALSE, correct = TRUE );
#   
#   pvalues[ iTrial ] = test$p.value; 
#   
# #  Sys.sleep(2)
# }
# #dev.off()