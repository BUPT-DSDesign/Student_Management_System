import { useEventStore } from "@/pinia/modules/event";
import { store } from '@/pinia/enter'


export const EventStore = new useEventStore(store)