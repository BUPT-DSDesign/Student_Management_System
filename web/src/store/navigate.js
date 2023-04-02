import { useNavigateStore } from "@/pinia/modules/navigate";
import { store } from '@/pinia/enter'

// 所有课程数据存储的地方
export const NavigateStore = new useNavigateStore(store)