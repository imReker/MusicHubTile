#pragma once
#ifndef rkMediaLib_h__
#define rkMediaLib_h__
#include "ZMedia/ZMediaLibClient.h"
#include "ZMedia/ZMediaQueueClient.h"

using namespace Platform;
namespace Reker {
	namespace RTHelper {
		namespace ZMedia {
			public enum class ZMediaState : int32 {
				Stopped, Playing, Paused
			};

			public ref class Artist sealed {
				friend ref class Song;
				friend ref class Album;
			public:
				property String^ Name { String^ get(); };
				property bool HasArt { bool get(); };
				static property Artist^ Empty { Artist^ get(); };
			public:
				String^ GetArtPath();
			private:
				ZMEDIAITEM m_Handle;
				String^ m_Name;
				static Artist^ m_Empty;
			private:
				Artist();
				Artist(ZMEDIAITEM handle);
			};

			public ref class Album sealed {
				friend ref class Song;
			public:
				property String^ Name { String^ get(); };
				property Reker::RTHelper::ZMedia::Artist^ Artist { Reker::RTHelper::ZMedia::Artist^ get(); };
				property bool HasArt { bool get(); };
				static property Album^ Empty { Album^ get(); };
			public:
				String^ GetArtPath();
			private:
				ZMEDIAITEM m_Handle;
				String^ m_Name;
				Reker::RTHelper::ZMedia::Artist^ m_Artist;
				static Album^ m_Empty;
			private:
				Album();
				Album(ZMEDIAITEM handle);
			};

			public ref class Song sealed {
				friend ref class MediaQueue;
			public:
				property String^ Name { String^ get(); };
				property Reker::RTHelper::ZMedia::Album^ Album { Reker::RTHelper::ZMedia::Album^ get(); };
				property Reker::RTHelper::ZMedia::Artist^ Artist { Reker::RTHelper::ZMedia::Artist^ get(); };
			private:
				ZMEDIAITEM m_Handle;
				String^ m_Name;
				Reker::RTHelper::ZMedia::Artist^ m_Artist;
				Reker::RTHelper::ZMedia::Album^ m_Album;
			private:
				Song(ZMEDIAITEM handle);
			};

			public ref class MediaQueue sealed {
			public:
				property ZMediaState State { ZMediaState get(); }
				property Song^ CurrentItem { Song^ get(); }
			public:
				static MediaQueue^ CreateMediaQueue();
				void Connect();
				void Disconnect();
			private:
				static MediaQueue^ m_Instance;
			private:
//				MediaQueue();
//				~MediaQueue();
			};

			public ref class Zune sealed {
			public:
				void Init();
				void Shutdown();
			private:
			};
		}
	}
}
#endif // rkMediaLib_h__
