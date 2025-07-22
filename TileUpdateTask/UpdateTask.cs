using System;
using Windows.ApplicationModel.Background;
using Windows.Data.Xml.Dom;
using Windows.Storage;
using Windows.UI.Notifications;
using Windows.UI.StartScreen;
using Reker.RTHelper.ZMedia;

namespace Reker.MusicHubTile.TileUpdateTask {
  public sealed class UpdateTask : IBackgroundTask {
    private const string SetAlwasyshowlivetile = "AlwaysShowLiveTile";

    private Zune _zuneApp = null;
    private MediaQueue _mediaQueue = null;

    public void Run(IBackgroundTaskInstance taskInstance) {
      if (!SecondaryTile.Exists("LiveMusicTile")) {
        return;
      }
      if (Initialize()) {
        if (_mediaQueue != null && _mediaQueue.State == ZMediaState.Playing) {
          // Get a deferral, to prevent the task from closing prematurely 
          // while asynchronous code is still running.
          BackgroundTaskDeferral deferral = taskInstance.GetDeferral();
          try {
            UpdateLiveTile();
          } catch (Exception) {
            ResetLiveTile();
          }
          // Inform the system that the task is finished.
          deferral.Complete();
        } else {
          bool alwaysShowLiveTile = false;
          var localSettings = ApplicationData.Current.LocalSettings.Values;
          if (localSettings.ContainsKey(SetAlwasyshowlivetile)) {
            alwaysShowLiveTile = (bool)localSettings[SetAlwasyshowlivetile];
          }
          if (!alwaysShowLiveTile) {
            ResetLiveTile();
          }
        }
        Shutdown();
      }
    }

    private bool Initialize() {
      if (_zuneApp == null) {
        try {
          _zuneApp = new Zune();
          _zuneApp.Init();
        } catch (Exception) {
          this._zuneApp = null;
          return false;
        }
      }
      if (_mediaQueue == null) {
        try {
          _mediaQueue = MediaQueue.CreateMediaQueue();
          _mediaQueue.Connect();
        } catch (Exception) {
          this._mediaQueue = null;
          return false;
        }
      }
      return true;
    }

    private void Shutdown() {
      if (_mediaQueue != null) {
        _mediaQueue.Disconnect();
      }
      if (_zuneApp != null) {
        _zuneApp.Shutdown();
      }
    }

    private static void ResetLiveTile() {
      try {
        TileUpdateManager.CreateTileUpdaterForSecondaryTile("LiveMusicTile").Clear();
      } catch (Exception) {
        // ignored
      }
    }

    private void UpdateLiveTile() {
      XmlDocument tileDom = new XmlDocument();

      tileDom.LoadXml(
                      @"<tile>
                          <visual version=""2"" branding=""none"">
                            <binding template=""TileSquare71x71_MusicVideo"">
                              <image id=""1"" src="""" />
                              <text id=""1""></text>
                              <text id=""2""></text>
                            </binding>
                            <binding template=""TileSquare150x150_MusicVideo"">
                              <image id=""1"" src="""" />
                              <text id=""1""></text>
                              <text id=""2""></text>
                            </binding>
                            <binding template=""TileWide310x150_MusicVideo"">
                              <image id=""1"" src="""" />
                              <text id=""1""></text>
                              <text id=""2""></text>
                            </binding>
                          </visual>
                        </tile>"
        );

      string imageUrl_1X1And_2X2 = null;
      string imageUrl_4X2 = null;

      imageUrl_1X1And_2X2 = imageUrl_4X2 = _mediaQueue.CurrentItem.Artist.GetArtPath();
      if (string.IsNullOrEmpty(imageUrl_1X1And_2X2)) {
        imageUrl_1X1And_2X2 = imageUrl_4X2 = "ms-appx:///Assets/Splash.png";
      }
      XmlNodeList nodes = tileDom.GetElementsByTagName("binding");
      foreach (IXmlNode node in nodes) {
        IXmlNode nodeImage = node.SelectSingleNode("image");
        if (node.Attributes.GetNamedItem("template").NodeValue.ToString() == "TileWide310x150_MusicVideo") {
          if (!string.IsNullOrEmpty(imageUrl_4X2)) {
            nodeImage.Attributes.GetNamedItem("src").NodeValue = imageUrl_4X2;
          }
        } else if (!string.IsNullOrEmpty(imageUrl_1X1And_2X2)) {
          nodeImage.Attributes.GetNamedItem("src").NodeValue = imageUrl_1X1And_2X2;
        }
        XmlNodeList textNodes = node.SelectNodes("text");
        textNodes[0].InnerText = textNodes[1].InnerText = _mediaQueue.CurrentItem.Artist.Name;
      }

      TileUpdater tileUpdater = TileUpdateManager.CreateTileUpdaterForSecondaryTile("LiveMusicTile");
      tileUpdater.Update(new TileNotification(tileDom));
    }
  }
}
