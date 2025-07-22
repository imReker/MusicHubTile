using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Navigation;
using Windows.ApplicationModel.Background;
using Windows.ApplicationModel.Store;
using Windows.Phone.ApplicationModel;
using Windows.Storage;
using Windows.System;
using Windows.UI.Notifications;
using Windows.UI.StartScreen;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Reker.MusicHubTile.Resources;
using Reker.MusicHubTile.TileUpdateTask;

namespace Reker.MusicHubTile {
  public partial class MainPage : PhoneApplicationPage {
    private const string TileArgumentFull = "?mode=Music";
    private const string TileArgumentTrial = "?mode=FromTile";
    private const string SetFullversion = "FullVersion";
    private const string SetAlwasyshowlivetile = "AlwaysShowLiveTile";

    private delegate void NavigatioinEventDelegate(NavigationEventArgs e);

    public static bool FullVersion = false;

    // 构造函数
    public MainPage() {
      InitializeComponent();
      var localSettings = ApplicationData.Current.LocalSettings.Values;
      if (localSettings.ContainsKey(SetFullversion)) {
        FullVersion = (bool)localSettings[SetFullversion];
      }
      if (localSettings.ContainsKey(SetAlwasyshowlivetile)) {
        tglAlwaysShowLiveTile.IsChecked = (bool)localSettings[SetAlwasyshowlivetile];
      }

      new Thread(Initialize) {IsBackground = true}.Start();

      if (Environment.OSVersion.Version < new Version(8, 10, 14141)) {
        tbLiveTileTip.Visibility = Visibility.Visible;
      }

      txtVersion.Text = AppResources.AppName + " v1.1.11";
    }

    private void Initialize() {
      CheckPurchase();
      if (SecondaryTile.Exists("LiveMusicTile")) {
        RegisterLiveTileUpdateTask();
      } else {
        UnregisterLiveTileUpdateTask();
      }
    }

    private void CheckPurchase() {
      bool isActive = CurrentApp.LicenseInformation.ProductLicenses[BuyPage.KEYFULLAPP].IsActive;
      if (isActive) {
        ApplicationData.Current.LocalSettings.Values[SetFullversion] = true;
      }
      if (FullVersion != isActive) {
        FullVersion = isActive;
        Dispatcher.BeginInvoke(new NavigatioinEventDelegate(OnNavigatedTo), new object[] {null});
      }
    }

    // 用于生成本地化 ApplicationBar 的示例代码
    private void BuildLocalizedApplicationBar() {
      // 将页面的 ApplicationBar 设置为 ApplicationBar 的新实例。
      ApplicationBar = new ApplicationBar {Mode = ApplicationBarMode.Minimized};

      // 创建新按钮并将文本值设置为 AppResources 中的本地化字符串。
      //ApplicationBarIconButton appBarButton = new ApplicationBarIconButton(new Uri("/Assets/AppBar/appbar.add.rest.png", UriKind.Relative));
      // appBarButton.Text = AppResources.AppBarButtonText;
      //ApplicationBar.Buttons.Add(appBarButton);

      // 使用 AppResources 中的本地化字符串创建新菜单项。
      ApplicationBarMenuItem appBarMenuItem;

      if (FullVersion == false) {
        appBarMenuItem = new ApplicationBarMenuItem(AppResources.MainPageAbmBuy);
      } else {
        appBarMenuItem = new ApplicationBarMenuItem(AppResources.MainPageAbmDonate);
      }
      ApplicationBar.MenuItems.Add(appBarMenuItem);
      appBarMenuItem.Click += abmBuy_Click;

      /*appBarMenuItem = new ApplicationBarMenuItem(AppResources.MainPageAbmAbout);
      ApplicationBar.MenuItems.Add(appBarMenuItem);
      appBarMenuItem.Click += abmAbout_Click;*/
    }

    protected override void OnNavigatedTo(NavigationEventArgs e) {
      if (e != null) {
        base.OnNavigatedTo(e);
        string mode;
        NavigationContext.QueryString.TryGetValue("mode", out mode);
        if (mode == "1" && NavigationService.CanGoBack) {
          NavigationService.RemoveBackEntry();
//          RefreshTiles();
        } else if (mode == "FromTile") {
          chkLaunchDirectly.Visibility = Visibility.Visible;
        }
      }

      BuildLocalizedApplicationBar();
      if (ApplicationProfile.Modes == ApplicationProfileModes.Default) {
        RefreshTiles();

        if (FullVersion) {
          CheckTiles(pnlLive);
          CheckTiles(pnlOpacity);
          CheckTiles(pnlTransparent);
          bdLaunchButton.Visibility = Visibility.Collapsed;
          chkLaunchDirectly.Visibility = Visibility.Collapsed;
        } else {
          bdLaunchButton.Visibility = Visibility.Visible;
        }
      } else {
        // Children mode
        bdLaunchButton.Visibility = Visibility.Visible;
      }
    }

    private void RefreshTiles() {
      try {
        SecondaryTile.FindAllAsync().Completed = (info, status) => {
          if (status == Windows.Foundation.AsyncStatus.Completed) {
            IReadOnlyList<SecondaryTile> tilelist = info.GetResults();
            foreach (SecondaryTile tile in tilelist) {
              tile.Arguments = FullVersion ? TileArgumentFull : TileArgumentTrial;
              tile.UpdateAsync();
            }
          }
        };
      } catch (Exception) {
        // ignored
      }
    }

    private static void UnregisterLiveTileUpdateTask() {
      string taskName = "MusicHubTile.LiveTilePlayItemChangedTask";
      UnregisterTask(taskName);
      taskName = "MusicHubTile.LiveTilePlayStateChangedTask";
      UnregisterTask(taskName);
    }

    private static async void RegisterLiveTileUpdateTask() {
      string taskName = "MusicHubTile.LiveTilePlayItemChangedTask";
      var trigger = new DeviceManufacturerNotificationTrigger("MediaQueue.ItemChanged.Notification", false);
      await RegisterTask(taskName, typeof (UpdateTask).FullName, trigger);

      taskName = "MusicHubTile.LiveTilePlayStateChangedTask";
      trigger = new DeviceManufacturerNotificationTrigger("MediaQueue.PlayStateChanged.Notification", false);
      await RegisterTask(taskName, typeof (UpdateTask).FullName, trigger);
    }

    private static void UnregisterTask(string taskName) {
      var oldtask = BackgroundTaskRegistration.AllTasks.Values.FirstOrDefault(task => task.Name == taskName);
      if (oldtask != null) {
        oldtask.Unregister(true);
      }
    }

    private static async Task RegisterTask(string taskName, string entryPoint, IBackgroundTrigger trigger) {
      UnregisterTask(taskName);
      var backgroundAccessStatus = await BackgroundExecutionManager.RequestAccessAsync();
      if (backgroundAccessStatus == BackgroundAccessStatus.AllowedMayUseActiveRealTimeConnectivity) {
        BackgroundTaskBuilder taskBuilder = new BackgroundTaskBuilder {Name = taskName, TaskEntryPoint = entryPoint};
        taskBuilder.SetTrigger(trigger);
        taskBuilder.Register();
        /*taskBuilder.Register().Completed += (sender, args) => {
          try {
            args.CheckResult();
          } catch (Exception) {
            // ignored
          }
        };*/
      }
    }

    private static void CheckTiles(DependencyObject obj) {
      for (int i = 0; i <= VisualTreeHelper.GetChildrenCount(obj) - 1; i++) {
        Button button = VisualTreeHelper.GetChild(obj, i) as Button;
        if (button == null) {
          continue;
        }
        string tileid = button.Name.Replace("btn", "");
        bool tileExists = SecondaryTile.Exists(tileid);
//        button.Visibility = tileExists ? Visibility.Collapsed : Visibility.Visible;
        button.IsEnabled = !tileExists;
      }
    }

    private void abmBuy_Click(object sender, EventArgs e) {
      NavigationService.Navigate(new Uri("/BuyPage.xaml?mode=" + (FullVersion ? "1" : "0"), UriKind.Relative));
    }

    /*private void abmAbout_Click(object sender, EventArgs e) {
      NavigationService.Navigate(new Uri("/AboutPage.xaml", UriKind.Relative));
    }*/

    private async void btnPin_Click(object sender, RoutedEventArgs e) {
      Button button = sender as Button;
      if (button == null) {
        return;
      }

      string tileId = button.Name.Replace("btn", "");

      if (SecondaryTile.Exists(tileId)) {
        return;
      }

      if (tileId.StartsWith("Live")) {
        if (FullVersion == false) {
          if (MessageBox.Show(AppResources.MsgNeedBuy, "", MessageBoxButton.OKCancel) == MessageBoxResult.OK) {
            abmBuy_Click(sender, e);
          }
          return;
        }
        
        RegisterLiveTileUpdateTask();
      }

      // Prepare package images for all four tile sizes in our tile to be pinned as well as for the square30x30 logo used in the Apps view.
      string tileFileName;
      Uri wide310X150Logo;
      if (tileId.StartsWith("OpacityMusicTile") || tileId.StartsWith("TransMusicTile") ||
          tileId.StartsWith("LiveMusicTile")) {
        tileFileName = "MusicTile";
        if (tileId.EndsWith("alt")) {
          wide310X150Logo = new Uri("ms-appx:///Assets/Tiles/MusicTile-large-alt.png");
        } else {
          wide310X150Logo = new Uri("ms-appx:///Assets/Tiles/MusicTile-large.png");
        }
      } else {
        if (tileId == "MusicTile2") {
          tileFileName = "MusicTile1";
        } else if (tileId == "MusicTile4") {
          tileFileName = "MusicTile3";
        } else {
          tileFileName = tileId;
        }
        wide310X150Logo = new Uri("ms-appx:///Assets/Tiles/" + tileId + "-large.png");
      }

      Uri square150X150Logo = new Uri("ms-appx:///Assets/Tiles/" + tileFileName + "-medium.png");
      Uri square71X71Logo = new Uri("ms-appx:///Assets/Tiles/" + tileFileName + "-small.png");

      // During creation of secondary tile, an application may set additional arguments on the tile that will be passed in during activation.
      // These arguments should be meaningful to the application. In this sample, we'll pass in the date and time the secondary tile was pinned.

      // Create a Secondary tile with all the required arguments.
      // Note the last argument specifies what size the Secondary tile should show up as by default in the Pin to start fly out.
      // It can be set to TileSize.Square150x150, TileSize.Wide310x150, or TileSize.Default.  
      // If set to TileSize.Wide310x150, then the asset for the wide size must be supplied as well.
      // TileSize.Default will default to the wide size if a wide size is provided, and to the medium size otherwise. 
      SecondaryTile secondaryTile = new SecondaryTile(
        tileId,
        AppResources.TileName,
        FullVersion ? TileArgumentFull : TileArgumentTrial,
        square150X150Logo,
        Windows.UI.StartScreen.TileSize.Square150x150);

      // Like the background color, the square30x30 logo is inherited from the parent application tile by default. 
      // Let's override it, just to see how that's done.
      secondaryTile.VisualElements.Wide310x150Logo = wide310X150Logo;
      secondaryTile.VisualElements.Square71x71Logo = square71X71Logo;

      // The display of the secondary tile name can be controlled for each tile size.
      // The default is false.
      secondaryTile.VisualElements.ShowNameOnSquare150x150Logo = true;
      secondaryTile.VisualElements.ShowNameOnWide310x150Logo = true;

      // Specify a foreground text value.
      // The tile background color is inherited from the parent unless a separate value is specified.
      secondaryTile.VisualElements.ForegroundText = ForegroundText.Dark;

      /*if (tileId.StartsWith("Opacity")) {
        Color clr = ((SolidColorBrush)button.Background).Color;
        secondaryTile.VisualElements.BackgroundColor = Windows.UI.Color.FromArgb(clr.A, clr.R, clr.G, clr.B);
      } else {
        secondaryTile.VisualElements.BackgroundColor = Windows.UI.Color.FromArgb(0, 0, 0, 0);
      }*/
      if (button.Background == Application.Current.Resources["PhoneAccentBrush"] as SolidColorBrush) {
        secondaryTile.VisualElements.BackgroundColor = Windows.UI.Color.FromArgb(0, 0, 0, 0);
      } else {
        Color clr = ((SolidColorBrush)button.Background).Color;
        secondaryTile.VisualElements.BackgroundColor = Windows.UI.Color.FromArgb(clr.A, clr.R, clr.G, clr.B);
      }

      // Set this to false if roaming doesn't make sense for the secondary tile.
      // The default is true;
      secondaryTile.RoamingEnabled = false;

      // OK, the tile is created and we can now attempt to pin the tile.

      // Since pinning a secondary tile on Windows Phone will exit the app and take you to the start screen, any code after 
      // RequestCreateForSelectionAsync or RequestCreateAsync is not guaranteed to run.  For an example of how to use the OnSuspending event to do
      // work after RequestCreateForSelectionAsync or RequestCreateAsync returns, see Scenario9_PinTileAndUpdateOnSuspend in the SecondaryTiles.WindowsPhone project.
      await secondaryTile.RequestCreateAsync();
    }

    private void btnOpenHub_Click(object sender, RoutedEventArgs e) {
      RTHelper.Shell.Launch();
      Application.Current.Terminate();
    }

    private async void btnHomePage_Click(object sender, RoutedEventArgs e) {
      await Launcher.LaunchUriAsync(new Uri(AppResources.HomePage));
    }

    private async void btnThanks1_Click(object sender, RoutedEventArgs e) {
      await Launcher.LaunchUriAsync(new Uri("https://twitter.com/marciozomb13"));
    }

    private async void btnThanks2_Click(object sender, RoutedEventArgs e) {
      await Launcher.LaunchUriAsync(new Uri("https://twitter.com/mdtauk"));
    }

    private async void btnRate_Click(object sender, RoutedEventArgs e) {
      string appId = CurrentApp.AppId.ToString();
      await Launcher.LaunchUriAsync(new Uri("zune:reviewapp?appid=" + appId));
    }

    private async void btnMore_Click(object sender, RoutedEventArgs e) {
      await Launcher.LaunchUriAsync(new Uri("zune:search?keyword=&publisher=reker&contenttype=app"));
    }

    private async void btnBingVisionTile_Click(object sender, RoutedEventArgs e) {
      await Launcher.LaunchUriAsync(new Uri("zune:navigate?appid=05894022-e18c-40a4-a6cc-992383aa7ee8"));
    }

    private void tglAlwaysShowLiveTile_Click(object sender, RoutedEventArgs e) {
      if (FullVersion == false) {
        if (MessageBox.Show(AppResources.MsgNeedBuy, "", MessageBoxButton.OKCancel) == MessageBoxResult.OK) {
          abmBuy_Click(sender, e);
        }
        tglAlwaysShowLiveTile.IsChecked = false;
        return;
      }
      bool value = tglAlwaysShowLiveTile.IsChecked != null && (bool)tglAlwaysShowLiveTile.IsChecked;
      ApplicationData.Current.LocalSettings.Values[SetAlwasyshowlivetile] = value;
      if (value == false) {
        if (SecondaryTile.Exists("LiveMusicTile")) {
          TileUpdateManager.CreateTileUpdaterForSecondaryTile("LiveMusicTile").Clear();
        }
      }
    }

    private void chkLaunchDirectly_Click(object sender, RoutedEventArgs e) {
      if (FullVersion == false) {
        if (MessageBox.Show(AppResources.MsgNeedBuy, "", MessageBoxButton.OKCancel) == MessageBoxResult.OK) {
          abmBuy_Click(sender, e);
        }
        chkLaunchDirectly.IsChecked = false;
      }
    }
  }
}
