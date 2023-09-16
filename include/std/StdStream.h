#ifndef STDSTREAM_H
#define STDSTREAM_H

#include "Stream.h"

extern Stream::IstreamStd stdinStream;  // stdin包装
extern Stream::OstreamStd stdoutStream; // stdout包装
extern Stream::OstreamStd stderrStream; // stderr包装

#endif