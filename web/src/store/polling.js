import { usePollingStore } from "@/pinia/modules/polling";
import { store } from '@/pinia/enter'

// 所有课程数据存储的地方
export const PollingStore = new usePollingStore(store)