import { useLogStore } from "@/pinia/modules/log";
import { store } from '@/pinia/enter'


export const LogStore = new useLogStore(store)