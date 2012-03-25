simple=read.table('simple.txt')
node=read.table('node.txt')

simple=cbind(simple,as.character(c('simple')))
colnames(simple)=c('time','type')
node=cbind(node,c('node'))
colnames(node)=c('time','type')

rez=data.frame(rbind(simple, node))

require(ggplot2)

ggplot(rez,aes(time,fill=type))+geom_density(alpha=0.6,size=1.3)+scale_x_log10()
