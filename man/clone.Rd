\name{clone}
\alias{clone}
\alias{clone-methods}
\alias{clone,GatingSet-method}
\title{clone a GatingSet}
\arguments{
  \item{x}{A \code{GatingSet}}

  \item{...}{ncdfFile = NULL: see
  \code{\link{clone.ncdfFlowSet}}}
}
\value{
  A copy of a given \code{GatingSet}.
}
\description{
  clone a GatingSet
}
\details{
  Note that the regular R assignment operation on a
  \code{GatingSet} object does not return the copy as one
  would normally expect because the \code{GatingSet}
  contains environment slots (and external pointer for
  \code{GatingSet}), which require deep-copying. So make
  sure to use this clone method in order to make a copy of
  existing object.
}
\examples{
\dontrun{
    #G is  a GatingSet
    G1<-clone(G)

  }
}

