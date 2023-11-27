#include "pch.h"
#include "UrlHelper.h"

UrlHelper::UrlHelper()
{
}

UrlHelper::UrlHelper(const std::string& url)
{
	parseUrl(url);
}

static bool seekComponent(const std::string& url, size_t& position, const std::string& endStr, std::string& result)
{
	size_t endPos = url.find(endStr, position);
	if (endPos == std::string::npos)
	{
		result = url.substr(position);
		return false;
	}

	result = url.substr(position, endPos - position);
	position = endPos + endStr.size();

	return true;
}

const std::string protocolEnd = "://";

void UrlHelper::parseUrl(const std::string& url)
{
	size_t position = 0;

	if (!seekComponent(url, position, protocolEnd, protocol))
		return;

	if (!seekComponent(url, position, "/", hostname))
		return;

	if (!seekComponent(url, position, "?", path))
		return;

	query = url.substr(position);
}

std::string UrlHelper::buildUrl()
{
	std::string url;

	if (protocol.empty() || hostname.empty())
		return url;

	url += protocol + protocolEnd + hostname;

	if (!path.empty())
		url += '/' + path;

	if (!query.empty())
	{
		if (path.empty())
			url += '/';
		url += '?' + query;
	}

	return url;
}
