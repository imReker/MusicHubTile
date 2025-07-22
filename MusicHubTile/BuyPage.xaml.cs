using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Windows.ApplicationModel.Store;
using Reker.MusicHubTile.Resources;
using Windows.Storage;

namespace Reker.MusicHubTile {
  public partial class BuyPage : PhoneApplicationPage {
    public static string KEYFULLAPP = "fullapp";
    public static string KEYDONATE = "donate";

    public BuyPage() {
      InitializeComponent();

      btnDonate1.Content += "$1";
      btnDonate2.Content += "$2";
      btnDonate5.Content += "$5";
      btnDonate20.Content += "$20";
      btnDonate50.Content += "$50";
      btnDonate200.Content += "$200";
      btnDonate500.Content += "$500";
    }

    protected override void OnNavigatedTo(NavigationEventArgs e) {
      string mode = "0";
      NavigationContext.QueryString.TryGetValue("mode", out mode);
      if (mode == "1") {
        stkBuy.Visibility = Visibility.Collapsed;
        stkDonate.Visibility = Visibility.Visible;
        tbTitle.Text = AppResources.BuyPageTitleDonate;

      } else {
        stkBuy.Visibility = Visibility.Visible;
        stkDonate.Visibility = Visibility.Collapsed;
      }
    }

    private async void btnBuy_Click(object sender, RoutedEventArgs e) {
      /*try {
        var ProdList = await CurrentApp.LoadListingInformationAsync();
        string t = "";

        foreach (var item in ProdList.ProductListings) {
          t = string.Format("{0}, {1}, {2},{3}, {4}",
                              item.Value.ProductId,
                              item.Value.Name,
                              item.Value.FormattedPrice,
                              item.Value.ProductType,
                              item.Value.Description);

          if (MessageBox.Show(t,"",MessageBoxButton.OKCancel)==MessageBoxResult.OK) {
            var Receipt = await CurrentApp.RequestProductPurchaseAsync(item.Value.ProductId, true);

            if (CurrentApp.LicenseInformation.ProductLicenses[item.Value.ProductId].IsActive) {
              // do someting with this license...

              // Notify the marketplace that the application has delivered the paid-for goods to the user. 
              CurrentApp.ReportProductFulfillment(item.Value.ProductId);
            }
            MessageBox.Show(Receipt, "", MessageBoxButton.OK);
            break;
          }
        }
      } catch (Exception ex) {
        MessageBox.Show("Error: " + ex.Message);
      }

      return;*/
      if (!CurrentApp.LicenseInformation.ProductLicenses[KEYFULLAPP].IsActive) {
        btnBuy.IsEnabled = false;
        PurchaseResults pr = null;
        try {

          ListingInformation listings = await CurrentApp.LoadListingInformationByProductIdsAsync(
                                        new string[] { KEYFULLAPP });


          pr = await CurrentApp.RequestProductPurchaseAsync(listings.ProductListings[KEYFULLAPP].ProductId);

        } catch (Exception) {
          MessageBox.Show(AppResources.MsgBuyFailed);
          (sender as Button).IsEnabled = true;
          return;
        }

        if (pr != null && (pr.Status == ProductPurchaseStatus.Succeeded ||
                            pr.Status == ProductPurchaseStatus.AlreadyPurchased) &&
            CurrentApp.LicenseInformation.ProductLicenses[KEYFULLAPP].IsActive) {
          BuyAppSucc();
        } else {
          MessageBox.Show(AppResources.MsgBuyFailed);
          btnBuy.IsEnabled = true;
          (sender as Button).IsEnabled = true;
        }

      } else {
        BuyAppSucc();
      }

    }

    private void BuyAppSucc() {
      MessageBox.Show(AppResources.MsgBuySuccess);
      ApplicationData.Current.LocalSettings.Values["FullVersion"] = true;
      MainPage.FullVersion = true;
      //NavigationService.Navigate(new Uri("/MainPage.xaml?mode=1", UriKind.Relative));
      if (NavigationService.CanGoBack) NavigationService.GoBack();
    }

    private async void btnDonate_Click(object sender, RoutedEventArgs e) {
      (sender as Button).IsEnabled = false;
      string strKey = KEYDONATE + (sender as Button).Name.Replace("btnDonate", "");
      PurchaseResults pr = null;
      try {
        ListingInformation listings = await CurrentApp.LoadListingInformationByProductIdsAsync(
                                      new string[] { strKey });


        pr = await CurrentApp.RequestProductPurchaseAsync(listings.ProductListings[strKey].ProductId);

        if (pr != null && pr.Status == ProductPurchaseStatus.Succeeded) {
          await CurrentApp.ReportConsumableFulfillmentAsync(strKey, pr.TransactionId);
          MessageBox.Show(AppResources.MsgBuySuccess);
        } else {
          MessageBox.Show(AppResources.MsgBuyFailed);
        }
      } catch (Exception) {
        MessageBox.Show(AppResources.MsgBuyFailed);
        return;
      } finally {
        (sender as Button).IsEnabled = true;
      }
    }

    private void btnDonate200_Hold(object sender, System.Windows.Input.GestureEventArgs e) {
      btnDonate500.Visibility = Visibility.Visible;
    }

  }
}