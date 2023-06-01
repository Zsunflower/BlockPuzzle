/****************************************************************************
Copyright (c) 2015-2017 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import android.app.Activity;
import android.app.Dialog;
import android.content.Intent;
import android.content.IntentSender;
import android.os.Bundle;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;

import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.InterstitialAd;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.api.Api;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.PendingResult;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.common.api.Status;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.leaderboard.LeaderboardScore;
import com.google.android.gms.games.leaderboard.LeaderboardVariant;
import com.google.android.gms.games.leaderboard.Leaderboards;

import android.preference.PreferenceManager;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.View;
import android.view.Gravity;
import android.widget.LinearLayout;

import java.io.FileDescriptor;
import java.io.PrintWriter;
import java.util.concurrent.TimeUnit;


public class AppActivity extends Cocos2dxActivity implements GoogleApiClient.ConnectionCallbacks,
        GoogleApiClient.OnConnectionFailedListener{
    private static final int RC_UNUSED = 5001;
    private AdView mAdView;
    private InterstitialAd mInterstitialAd;
    private static AppActivity _appActiviy;
    public native void callbackOnAdLoaded();
    public native void callbackOnAdFailedToLoad();
    public native void callbackOnAdOpened();
    public native void callbackOnAdLeftApplication();
    public native void callbackOnAdClosed();
    public native static void callbackOnReciveScore(long highScore);

    private static GoogleApiClient mGoogleApiClient;
    public static String LEADERBOARD_ID = "CgkI_6Hm-KUDEAIQAA";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);
        // Workaround in https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root activity into an existing task
            //  so just quietly finish and go away, dropping the user back into the activity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }
        // DO OTHER INITIALIZATION BELOW
        this.getGLSurfaceView().setMultipleTouchEnabled(false);
        MobileAds.initialize(getApplicationContext(),
                "ca-app-pub-5118564015725949~1863675501");
        mAdView = new AdView(this);
        mAdView.setAdSize(AdSize.SMART_BANNER);
        mAdView.setAdUnitId("ca-app-pub-5118564015725949/8018020319");
        AdRequest adRequest = new AdRequest.Builder()
                .addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
                .addTestDevice("E556E0363B23268E935BB12FAEE71D3E")
                .build();
        mAdView.loadAd(adRequest);
        LinearLayout linearLayout = new LinearLayout(this);
        LinearLayout.LayoutParams adParams = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT);
        adParams.gravity = Gravity.BOTTOM;
        linearLayout.addView(mAdView, adParams);
        mFrameLayout.addView(linearLayout);

        mInterstitialAd = new InterstitialAd(this);
        mInterstitialAd.setAdUnitId("ca-app-pub-5118564015725949/5507885972");
        mInterstitialAd.setAdListener(new AdListener() {
            @Override
            public void onAdLoaded() {
                // Code to be executed when an ad finishes loading.
                callbackOnAdLoaded();
            }

            @Override
            public void onAdFailedToLoad(int errorCode) {
                // Code to be executed when an ad request fails.
                callbackOnAdFailedToLoad();
            }

            @Override
            public void onAdOpened() {
                // Code to be executed when the ad is displayed.
                callbackOnAdOpened();
            }

            @Override
            public void onAdLeftApplication() {
                // Code to be executed when the user has left the app.
                callbackOnAdLeftApplication();
            }

            @Override
            public void onAdClosed() {
                // Code to be executed when when the interstitial ad is closed.
                callbackOnAdClosed();
            }
        });
        mInterstitialAd.loadAd(new AdRequest.Builder().build());

        mGoogleApiClient = new GoogleApiClient.Builder(getApplicationContext())
                .addConnectionCallbacks(this)
                .addOnConnectionFailedListener(this)
                .addApi(Games.API).addScope(Games.SCOPE_GAMES)
                .setViewForPopups(findViewById(android.R.id.content))
                .build();

        _appActiviy = this;
    }

    @Override
    protected void onStart() {
        super.onStart();
    }

    public static boolean isSignedIn()
    {
        return (mGoogleApiClient != null && mGoogleApiClient.isConnected());
    }

    public static boolean isSignedOut()
    {
        return !isSignedIn();
    }

    public static void signOut()
    {
        if(isSignedIn())
        {
            Games.signOut(mGoogleApiClient);
            mGoogleApiClient.disconnect();
            Log.d("==========", "SignOut Google play services");
        }
    }

    public static void signIn()
    {
        if(isSignedOut())
        {
            Log.d("==========", "SignIn Google play services");
            mGoogleApiClient.connect();
        }
    }

    @Override
    public void onConnected(@Nullable Bundle bundle) {
        Log.d("============", "onConnected");
    }

    public static void showLeaderBoard()
    {
        if(isSignedIn())
            _appActiviy.startActivityForResult(Games.Leaderboards.getLeaderboardIntent(mGoogleApiClient, LEADERBOARD_ID), 100);
    }

    @Override
    public void onConnectionFailed(@NonNull ConnectionResult connectionResult) {
        Log.d("============", "onConnectionFailed" + connectionResult);
        if (connectionResult.hasResolution()) {
            try {
                connectionResult.startResolutionForResult(this, 111);
            } catch (Exception e) {
                mGoogleApiClient.connect();
            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(requestCode == 111)
        {
            if(resultCode == RESULT_OK)
                mGoogleApiClient.connect();
        }
    }

    @Override
    public void onConnectionSuspended(int i) {
        Log.d("============", "onConnectionSuspended");
    }

    public static void submitScore(long score)
    {
        if(isSignedIn())
        {
            Games.Leaderboards.submitScore(mGoogleApiClient, LEADERBOARD_ID, score);
        }
    }

    public static void getHighScore()
    {
        if(isSignedIn())
        Games.Leaderboards.loadCurrentPlayerLeaderboardScore(mGoogleApiClient, LEADERBOARD_ID, LeaderboardVariant.TIME_SPAN_ALL_TIME,
                LeaderboardVariant.COLLECTION_PUBLIC).setResultCallback
                (
                new ResultCallback<Leaderboards.LoadPlayerScoreResult>()
                {
                    @Override
                    public void onResult(Leaderboards.LoadPlayerScoreResult arg0)
                    {
                        LeaderboardScore c = arg0.getScore();
                        if(c != null) {
                            long highScore = c.getRawScore();
                            callbackOnReciveScore(highScore);
                        }
                    }
                }
                );
    }

    public static String myGetPackageName()
    {
        return Cocos2dxHelper.getCocos2dxPackageName();
    }

    public static void hideAd() {
        _appActiviy.runOnUiThread(new Runnable() {

            @Override
            public void run() {
                if (_appActiviy.mAdView.isEnabled())
                    _appActiviy.mAdView.setEnabled(false);
                if (_appActiviy.mAdView.getVisibility() != View.INVISIBLE)
                    _appActiviy.mAdView.setVisibility(View.INVISIBLE);
            }
        });
    }

    public static void showAd() {
        _appActiviy.runOnUiThread(new Runnable() {

            @Override
            public void run() {
                if (!_appActiviy.mAdView.isEnabled())
                    _appActiviy.mAdView.setEnabled(true);
                if (_appActiviy.mAdView.getVisibility() != View.VISIBLE)
                    _appActiviy.mAdView.setVisibility(View.VISIBLE);
            }
        });
    }

    public static void showInterstitialAd() {

        _appActiviy.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (_appActiviy.mInterstitialAd.isLoaded()) {
                    _appActiviy.mInterstitialAd.show();
                } else {
                    _appActiviy.mInterstitialAd.loadAd(new AdRequest.Builder().build());
                }
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (mAdView != null) {
            mAdView.resume();
        }
    }

    @Override
    protected void onPause() {
        if (mAdView != null) {
            mAdView.pause();
        }
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        mAdView.removeAllViews();
        mAdView.destroy();
        super.onDestroy();
    }
}