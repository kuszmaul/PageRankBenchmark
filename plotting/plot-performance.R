cppdata <- read.table("../code/C++/c++.data", header=TRUE, check.names=FALSE, sep="\t", row.names=NULL)
pdf("K0.pdf")
boxplot(K0~scale, data=cppdata, notch=TRUE, xlab="SCALE", ylab="Edges/s", main="K0 C++ Performance")
dev.off()

pdf("K1.pdf")
boxplot(K1~scale, data=cppdata, notch=TRUE, xlab="SCALE", ylab="Edges/s", main="K1 C++ Performance")
dev.off()

pdf("K2.pdf")
boxplot(K2~scale, data=cppdata, notch=TRUE, xlab="SCALE", ylab="Edges/s", main="K2 C++ Performance")
dev.off()

pdf("K3.pdf")
boxplot(K3~scale, data=cppdata, notch=TRUE, xlab="SCALE", ylab="Edges/s", main="K3 C++ Performance")
dev.off()
