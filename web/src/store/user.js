import { useUserStore } from "@/pinia/modules/user";
import { store } from '@/pinia/enter'

// 所有用户信息储存的地方
export const UserStore = new useUserStore(store)