#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <cpprest/json.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

//Function definition, can define functions in .cpp or .h file
void leagueFinder();

int wmain() {
	leagueFinder();
	system("pause");
}

void leagueFinder() {
	
	//Define the URL, name, and key
	utility::string_t url = L"https://na1.api.riotgames.com/lol/summoner/v3/summoners/by-name/";
	utility::string_t name = L"";
	utility::string_t key = L"?api_key=RGAPI-6bb999dc-0aea-4aa3-98d8-c81764b0784a";

	//Get Summoner name
	std::cout << "Summoner: ";
	std::wcin >> name;
	std::cout << "\n";

	//Add name and api key to url
	url += name + key;

	//Define http client
	http_client client1(url);

	//Make request
	pplx::task<http_response> response = client1.request(methods::GET);

	//Additional steps to convert client response to JSON
	http_response data = response.get();
	pplx::task<json::value> json = data.extract_json();
	json::value& riotData = json.get();

	//Print JSON object
	std::wcout << "JSON object: " << riotData << "\n" << std::endl;

	//Parse JSON for 'id' tag
	int summonerID = riotData.at(U("id")).as_integer();

	std::cout << "Summoner ID: " << summonerID << "\n" << std::endl;



	//Make another HTTP call
	url = L"https://na1.api.riotgames.com/lol/league/v3/positions/by-summoner/";

	//TODO: append summonerID to url and api key
	std::string s = std::to_string(summonerID);
	url += utility::conversions::to_string_t(s) + key;

	//Define http client for obtaining a player's rank
	http_client client2(url);

	//Make request
	response = client2.request(methods::GET);

	//Additional steps to convert client response to JSON
	data = response.get();
	json = data.extract_json();
	riotData = json.get();

	std::wcout << "JSON object: " << riotData << "\n" << std::endl;

	//Parse JSON for 'tier' tag
	auto summonerRank = riotData[0].at(U("tier")).as_string();

	std::wcout << "Summoner Tier: " << summonerRank << "\n";
}