import Vue from 'vue'
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
import App from './App.vue'
import router from './router';
import axios from 'axios'
import AMap from 'vue-amap'
import { createPinia } from 'pinia'

// 将axios挂载到原型上，以供全局使用
Vue.prototype.$http = axios 

// 使用ElementUI设计
Vue.use(ElementUI);

// 使用pinia模块管理
const store = new createPinia()
Vue.use(store)

// 使用高德地图实现导航
Vue.use(AMap)
AMap.initAMapApiLoader({
  key: '645785d3ed8cd58933f1e27cd08672cb',
  plugin: ['AMap.Autocomplete', 'AMap.PlaceSearch', 'AMap.Scale', 'AMap.OverView', 'AMap.ToolBar', 'AMap.MapType', 'AMap.PolyEditor', 'AMap.CircleEditor'],
  // 默认高德 sdk 版本为 1.4.4
  v: '1.4.4'
})

Vue.config.productionTip = false

new Vue({
  router,
  render: h => h(App),
}).$mount('#app')
