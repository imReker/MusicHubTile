using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Reker.MusicHubTile.Resources;
using Windows.System;

namespace Reker.MusicHubTile {
  public partial class AboutPage : PhoneApplicationPage {
    public AboutPage() {
      InitializeComponent();
      txtVersion.Text = AppResources.AppName + " v" + System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString(); 
    }
    private async void btnHomePage_Click(object sender, RoutedEventArgs e) {
      await Launcher.LaunchUriAsync(new Uri(AppResources.HomePage));
    }

    private async void btnRate_Click(object sender, RoutedEventArgs e) {
      string appID = Windows.ApplicationModel.Store.CurrentApp.AppId.ToString();
      await Launcher.LaunchUriAsync(new Uri("zune:reviewapp?appid=" + appID));
    }

    private async void btnMore_Click(object sender, RoutedEventArgs e) {
      await Launcher.LaunchUriAsync(new Uri("zune:search?keyword=&publisher=reker&contenttype=app"));
    }

    private async void HyperlinkButton1_Click(object sender, RoutedEventArgs e) {
      await Launcher.LaunchUriAsync(new Uri("https://twitter.com/marciozomb13"));
    }

    private async void HyperlinkButton2_Click(object sender, RoutedEventArgs e) {
      await Launcher.LaunchUriAsync(new Uri("https://twitter.com/mdtauk"));
    }
  }
}