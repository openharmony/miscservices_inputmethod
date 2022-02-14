import router from '@ohos.router'
import resourceManager from '@ohos.resourceManager';

export default {
    data: {
        imeList: router.getParams().imeList,
    },
    onInit() {
        this.dialogTitle = this.$t('strings.dialogTitle');
        this.setIme = this.$t('strings.setIme');
        this.initString()
    },
    changeDefaultIme(ime) {
        console.info('ImsaKit-dialog changeDefaultIme: ' + ime);
        callNativeHandler("EVENT_CHANGE_IME", ime);
    },
    startImeSetting() {
        console.info('ImsaKit-dialog startImeSetting');
        callNativeHandler("EVENT_START_IME_SETTING", "");
    },
    initString() {
        for (var i = 0; i < this.imeList.length; i++) {
            let bundle = this.imeList[i].ime;
            console.info('ImsaKit-dialog initString bundle ' + bundle);
            let index = bundle.indexOf("/");
            let pn = bundle.substring(0, index);
            let labelId = Number(this.imeList[i].labelId);
            let discriptionId = Number(this.imeList[i].discriptionId);
            resourceManager.getResourceManager(pn).then(mgr => {
                mgr.getString(labelId).then(value => {
                    this.updateLabelData(bundle, value, '');
                }).catch(error => {
                    console.info("ImsaKit-dialog initString resource getString error:" + error);
                })
                mgr.getString(discriptionId).then(value => {
                    this.updateLabelData(bundle, '', value);
                }).catch(error => {
                    console.info("ImsaKit-dialog initString resource getString error:" + error);
                })
            }).catch(error => {
                console.info("ImsaKit-dialog initString getResourceManager error:" + error);
            });
        }
    },
    updateLabelData(bundle, label, discription) {
        for (var i = 0; i < this.imeList.length; i++) {
            if (this.imeList[i].ime == bundle) {
                if (label != '') {
                    this.imeList[i].label = label;
                }
                if (discription != '') {
                    this.imeList[i].discription = discription;
                }
            }
        }
    }
}
