#pragma once

class UrlHelper
{
public:
	std::string protocol, hostname, path, query;

	UrlHelper();
	UrlHelper(const std::string& url);

	void parseUrl(const std::string& url);
	std::string buildUrl();
};
