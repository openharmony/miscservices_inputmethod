import router from '@ohos.router'

export default {
    data: {
        imeList: router.getParams().imeList,
    },
    onInit() {
        this.dialogTitle = this.$t('strings.dialogTitle');
        this.setIme = this.$t('strings.setIme');
    },
    changeDefaultIme(ime) {
        console.info('ImsaKit-dialog changeDefaultIme: ' + ime);
        callNativeHandler("EVENT_CHANGE_IME", ime);
    },
    startImeSetting() {
        console.info('ImsaKit-dialog startImeSetting');
        callNativeHandler("EVENT_START_IME_SETTING", "");
    },
}
