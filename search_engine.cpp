// search_engine.cpp: определяет точку входа для приложения.
//

#include "search_engine.h"
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

using namespace N;
using namespace std;

#if defined(_WIN64)
#define PLATFORM_NAME "x64" // Windows
#elif defined(_WIN32)
#define PLATFORM_NAME "windows" // Windows
#elif defined(__CYGWIN__) && !defined(_WIN32)
#define PLATFORM_NAME "windows" // Windows (Cygwin POSIX under Microsoft Window)
#elif defined(__ANDROID__)
#define PLATFORM_NAME "android" // Android (implies Linux, so it must come first)
#elif defined(__linux__)
#define PLATFORM_NAME "linux" // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#elif defined(__unix__) || !defined(__APPLE__) && defined(__MACH__)
#include <sys/param.h>
#if defined(BSD)
#define PLATFORM_NAME "bsd" // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
#endif
#elif defined(__hpux)
#define PLATFORM_NAME "hp-ux" // HP-UX
#elif defined(_AIX)
#define PLATFORM_NAME "aix" // IBM AIX
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#define PLATFORM_NAME "ios" // Apple iOS
#elif TARGET_OS_IPHONE == 1
#define PLATFORM_NAME "ios" // Apple iOS
#elif TARGET_OS_MAC == 1
#define PLATFORM_NAME "osx" // Apple OSX
#endif
#elif defined(__sun) && defined(__SVR4)
#define PLATFORM_NAME "solaris" // Oracle Solaris, Open Indiana
#else
#define PLATFORM_NAME NULL
#endif

// Return a name of platform, if determined, otherwise - an empty string
const char* get_platform_name() {
	return (PLATFORM_NAME == NULL) ? "" : PLATFORM_NAME;
}


int main()
{
	ConverterJSON* cjs = new ConverterJSON();
	InvertedIndex* idx = new InvertedIndex();
	vector<string> docs, requests;
	std::vector<std::vector<RelativeIndex>> ans;
	setlocale(LC_ALL, "Russian");

	std::cout << (get_platform_name()) << "  \n";

	docs = cjs->GetTextDocuments();
	// for (int i = 0; i < docs.size(); i++) std::cout << docs[i] << "  i= " << i << "  \n";
	int max_responses = cjs->GetResponsesLimit();
	int time_update = cjs->GetTimeUpdate();
	std::cout << "  max_responses: " << max_responses << "  \n";
	std::cout << "  time_update: " << time_update << "  \n";
	requests = cjs->GetRequests();
	// for (int i = 0; i < requests.size(); i++) std::cout << requests[i] << "  i= " << i << "  requests\n";

	idx->UpdateDocumentBase(docs);
	SearchServer* ssrv = new SearchServer(*idx);
	ans = ssrv->make_ans(requests);
	cjs->putAnswers(ans);
	std::cout << "  Answers:  " << ans.size() << " \n";
	
	char c;
	long l0 = time(nullptr);
	long n = time_update;
	while (true) {
		long l = time(nullptr);
		if (l > l0) {
			n += l0 - l;
			long sec = n % 60;
			long min = n / 60;
			if (min > 0) std::cout << min << " min " << sec << " sec \n";
			else std::cout << sec << " sec "  << "\n";
			l0 = l;
			if (n <= 0) {
				
				std::cout << " Update/exit?  any key /n \n";
				std::cin >> c;
				if (c == 'n') exit(1);
				idx->UpdateDocumentBase(docs);
				SearchServer* ssrv = new SearchServer(*idx);
				ans = ssrv->make_ans(requests);
				cjs->putAnswers(ans);
				n = time_update; 
				l0 = time(nullptr);
			}
		}
	}
	return 0;
}
