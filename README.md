# Privacy-Preserving distributed sparse GP regression
In this work, we propose a Privacy Preserving Distributed Sparse local and global GP approximation with multi-key HE to ensure privacy of the individual devices' data in distributed/IoT computing scenarios. Each device first prepares its local summary using its private training data. The local summary comprise of a mean vector $\mu$ of size $(M)$ for $(M)$ samples in training dataset and a covariance matrix $(\Sigma)$ of size $(M\times M)$. It is then encrypted using a public encryption key $(\tilde{b})$ and sent over to the server for aggregation to compute sum of all the encrypted local summaries into an encrypted global summary. To decrypt the global summary, each device first computes a decryption share $(D_i)$, which is then combined at the server to get the final decryption result which is then used to perform predictive evaluation.
# Libraries
HEAAN V2.1.0: https://github.com/snucrypto/HEAAN
NTL 11.5.1: https://github.com/libntl/ntl
GMP 6.2.1: https://gmplib.org/}
