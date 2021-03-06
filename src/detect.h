#pragma once

#include "document.h"

namespace fasttext {
    class FastText;
}

boost::optional<std::string> DetectLanguage(const fasttext::FastText& model, const TDocument& document);
ENewsCategory DetectCategory(const fasttext::FastText& model, const TDocument& document);
