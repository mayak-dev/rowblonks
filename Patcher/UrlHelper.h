#pragma once

class UrlHelper
{
public:
	std::string protocol, hostname, path, query;

	UrlHelper();
	UrlHelper(const std::string& url);

	void ParseUrl(const std::string& url);
	std::string BuildUrl();
};
