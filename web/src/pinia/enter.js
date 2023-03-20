// 使用pinia模块管理
import { createPinia } from "pinia"
import piniaPluginPersistedstate from 'pinia-plugin-persistedstate'

export const store = new createPinia()
store.use(piniaPluginPersistedstate)