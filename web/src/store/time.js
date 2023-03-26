import { useTimeStore } from '@/pinia/modules/time';
import { store } from '@/pinia/enter'

export const TimeStore = new useTimeStore(store)

