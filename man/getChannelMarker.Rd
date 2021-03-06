\name{getChannelMarker}
\alias{getChannelMarker}
\title{get channel and marker information from a \code{flowFrame} that matches to the given keyword}
\usage{
  getChannelMarker(frm, name, ...)
}
\arguments{
  \item{frm}{\code{flowFrame} object}

  \item{name}{\code{character} the keyword to match}

  \item{...}{other arguments: not used.}
}
\value{
  an one-row \code{data.frame} that contains "name"(i.e.
  channel) and "desc"(i.e. stained marker) columns.
}
\description{
  This function tries best to guess the flow parameter
  based on the keyword supplied by \code{name} It first
  does a complete word match(case insensitive) between
  \code{name} and flow channels and markers. If there are
  duplcated matches, throw the error. If no matches, it
  will try the partial match.
}

